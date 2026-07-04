#include "KitchenCounterState.h"
#include "PlayState.h"
#include <iostream>
#include <cmath>
#include "TutorialState.h"


KitchenCounterState::KitchenCounterState(StateManager& manager) : manager(manager), cookbookText(font)
{
    if (!font.openFromFile("Fonts/Super Starfish.ttf"))
    {
        std::cout << "Failed to load font\n";
    }

    //cookbookbutton
    cookbookButton.setSize({ 150.f, 50.f });
    cookbookButton.setPosition({ 350.f,150.f });
    cookbookButton.setFillColor(sf::Color(34, 139, 34));



    cookbookText.setFont(font);
    cookbookText.setCharacterSize(22);
    cookbookText.setFillColor(sf::Color::White);
    cookbookText.setString("CookBook");

    sf::FloatRect bounds = cookbookText.getLocalBounds();
    cookbookText.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
        });

    cookbookText.setPosition({
         cookbookButton.getPosition().x + cookbookButton.getSize().x / 2.f,
           cookbookButton.getPosition().y + cookbookButton.getSize().y / 2.f
        });
    
    if (!texture.loadFromFile("Texture/counter_layout.png")) // <= background
    {
        std::cout << "Failed to load counter texture\n";
    }

    //creature

    creature.setSize({ 280.f, 280.f });
    creature.setTexture(&manager.creatureDefaultTexture);

    // left side, slightly up
    creature.setPosition({ 20.f, 120.f });

    background.setSize({ 960, 720 });
    background.setTexture(&texture);
    
    // Cutting board area (adjust if needed)
     cuttingBoardArea = sf::FloatRect({ 200.f, 200.f }, { 450.f, 400.f });

     //spritesheet

     if (!toolsTexture.loadFromFile("Texture/tools_spritesheet.png"))
     {
         std::cout << "Failed to load tools spritesheet\n";
     }

     // adding the tools

     toolItems.emplace_back(toolsTexture, sf::IntRect({ 0,0 }, { 605,560 }), ToolType::Knife);
     toolItems.emplace_back(toolsTexture, sf::IntRect({ 605,0 }, { 605,560 }), ToolType::Peeler);

     // position for the tools

     for (int i = 0; i < toolItems.size(); i++)
     {
         toolItems[i].sprite.setPosition({ 10.f + i * 90.f, 630.f });
         toolItems[i].sprite.setScale({ 0.1f, 0.1f });
     }

     //cursor

     toolCursor.setTexture(&toolsTexture);
     toolCursor.setSize({ 150.f, 150.f });
     toolCursor.setOrigin({
         toolCursor.getSize().x / 2.f,
         toolCursor.getSize().y / 2.f
         });


     //tuorial

     tutorialArrow.setTexture(&manager.arrowJugHintTexture);
     tutorialArrow.setSize({ 100.f,100.f });
     tutorialArrow.setOrigin({ 50.f,50.f });
     tutorialArrow.setRotation(sf::degrees(-45.f));


     tutorialArrow.setFillColor(sf::Color::Yellow);

     peelLine.setTexture(&manager.peelHintTexture);
     peelLine.setSize({ 340.f,220.f });
     peelLine.setFillColor(sf::Color::Yellow);


     cutLine.setTexture(&manager.cutHintTexture);
     cutLine.setSize({ 280.f,220.f });
     cutLine.setRotation(sf::degrees(-85.f));
     cutLine.setFillColor(sf::Color::Yellow);

     knifeFeedback.setTexture(&manager.knifeHintTexture);
     knifeFeedback.setSize({ 200.f,200.f });
     knifeFeedback.setOrigin({ 60.f,60.f });
     knifeFeedback.setFillColor(sf::Color::Yellow);

     peelerFeedback.setTexture(&manager.peelerHintTexture);
     peelerFeedback.setSize({ 200.f,200.f });
     peelerFeedback.setOrigin({ 60.f,60.f });
     peelerFeedback.setFillColor(sf::Color::Yellow);

     if (manager.counterTutorialFinished)
     {
         counterTutorialStage = CounterTutorialStage::Finished;
     }
     else
     {
         counterTutorialStage = CounterTutorialStage::DragToBoard;
     }

     //sfx

     if (!cutBuffer.loadFromFile("SFX/cut.wav"))
         std::cout << "Failed to load cut sound\n";

     if (!peelBuffer.loadFromFile("SFX/peel.wav"))
         std::cout << "Failed to load peel sound\n";

     cutSound.emplace(cutBuffer);
     peelSound.emplace(peelBuffer);

     cutSound.value().setVolume(100.f);
     peelSound.value().setVolume(100.f);

}

void KitchenCounterState::handleEvent(sf::RenderWindow& window, const sf::Event& event)
{

    if (event.is<sf::Event::KeyPressed>())
    {
        auto key = event.getIf<sf::Event::KeyPressed>();

        if (key && key->code == sf::Keyboard::Key::Escape)
        {
            //stops the item apearing on the hand when esc
            if (auto* dragged = manager.inventory.getDraggedItem())
                dragged->isDragging = false;

            manager.setState(std::make_unique<PlayState>(manager));

            showToolCursor = false;
            window.setMouseCursorVisible(true);

        }
    }




    // mouse press
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        //inventory drag
        //manager.inventory.handleEvent(event);

        auto mouse = event.getIf<sf::Event::MouseButtonPressed>();

        if (mouse->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos(
                (float)mouse->position.x,
                (float)mouse->position.y
            );

            if (cookbookButton.getGlobalBounds().contains(mousePos))
            {
                manager.setState(std::make_unique<TutorialState>(
                    manager,
                    manager.tutorialPages,
                    TutorialReturn::Counter));

                return;
            }

            Item* clickedItem = manager.inventory.getItemAt(mousePos);

            // BLOCK dragging the jug
            if (dynamic_cast<Jug*>(clickedItem))
            {
                manager.wrongBuzzSound.value().play();

                setCreatureMood(CreatureMood::Angry);
                creatureClock.restart();

                std::cout << "Can't use the jug here!\n";
            }
            else if (clickedItem && currentTool == ToolType::None)
            {
                pendingDragItem = clickedItem;
                pressStartPos = mousePos;
                mouseHeld = true;
            }

            dragStart = mousePos;
            isDraggingGesture = true;
            
            for (auto& tool : toolItems) // <= clicking on the tool
            {
                if (tool.sprite.getGlobalBounds().contains(mousePos))
                {
                    currentTool = tool.type;

                    setCreatureMood(CreatureMood::Scared);
                    creatureClock.restart();

                    showToolCursor = true;

                    toolCursor.setTextureRect(tool.sprite.getTextureRect());

                    window.setMouseCursorVisible(false);

                    manager.selectSound.value().play();
                    std::cout << "Tool selected!\n";
                    return; 
                }
            }

            // selecting the ingredient
            for (auto& item : counterItems)
            {
                if (item->sprite.getGlobalBounds().contains(mousePos))
                {
                    if (auto* ingredient = dynamic_cast<Ingredient*>(item.get()))
                    {
                        selectedIngredient = ingredient;

                        //selectedIngredient = item.get();

                        // ONLY DRAG if NO tool selected
                        if (currentTool == ToolType::None)
                        {
                           // item->isDragging = true;
                           // item->dragOffset = item->sprite.getPosition() - mousePos;

                            pendingDragItem = item.get();
                            pressStartPos = mousePos;
                            mouseHeld = true;
                        }
                    }
                    else
                    {
                        // It's NOT an ingredient (probably a Pot)
                        //selectedIngredient = nullptr;

                        // Still allow dragging!
                        //item->isDragging = true;
                        //item->dragOffset = item->sprite.getPosition() - mousePos;

                        selectedIngredient = nullptr;


                        if (currentTool == ToolType::None)
                        {
                            pendingDragItem = item.get();
                            pressStartPos = mousePos;
                            mouseHeld = true;
                        }
                    }

                    return;
                }
            }
        }


    }

   if (event.is<sf::Event::MouseMoved>())
    {
        auto mouse = event.getIf<sf::Event::MouseMoved>();

        currentMousePos = {
      (float)mouse->position.x,
      (float)mouse->position.y
        };

        toolCursor.setPosition(currentMousePos);
     
        manager.inventory.setMousePosition(currentMousePos);

        if (mouseHeld && pendingDragItem && !pendingDragItem->isDragging)
        {
            sf::Vector2f delta = currentMousePos - pressStartPos;

            if (std::hypot(delta.x, delta.y) > 10.f)
            {
                pendingDragItem->isDragging = true;

                pendingDragItem->dragOffset =
                    pendingDragItem->sprite.getPosition() - currentMousePos;

                mouseHeld = false;

                pendingDragItem = nullptr;
            }
        }
    }



    // dropinng and moving mechanic
    if (event.is<sf::Event::MouseButtonReleased>())
    {
        auto mouse = event.getIf<sf::Event::MouseButtonReleased>();


        if (!mouse)
            return;


        if (mouse->button != sf::Mouse::Button::Left)
            return;

        sf::Vector2f mousePos((float)mouse->position.x, (float)mouse->position.y);

        dragEnd = mousePos;

        // Mouse action movement logic
        if (isDraggingGesture && selectedIngredient)
        {
            sf::Vector2f delta = dragEnd - dragStart;

            float dx = std::abs(delta.x);
            float dy = std::abs(delta.y);

            if (std::hypot(dx, dy) > 30.f) // ignore tiny movement
            {
                //CUT (diagonal)
                if (currentTool == ToolType::Knife && dx > 20.f && dy > 20.f)
                {
                    if (canCut(selectedIngredient->type))
                    {
                        bool canCutNow = false;

                        if (selectedIngredient->type == IngredientType::Carrot ||
                            selectedIngredient->type == IngredientType::Parsnip ||
                            selectedIngredient->type == IngredientType::Cereliac)
                        {
                            canCutNow = (selectedIngredient->state == IngredientState::Peeled);
                        }
                        else
                        {
                            canCutNow = (selectedIngredient->state == IngredientState::Whole);
                        }
                        if (selectedIngredient->state == IngredientState::Cut)
                        {
                            manager.wrongBuzzSound.value().play();
                            std::cout << "Already cut!\n";
                            currentTool = ToolType::None;

                            setCreatureMood(CreatureMood::Angry);
                            creatureClock.restart();

                            showToolCursor = false;
                            window.setMouseCursorVisible(true);
                        }
                        else if (canCutNow)
                        {
                            selectedIngredient->state = IngredientState::Cut;
                            selectedIngredient->updateSprite();

                            if (manager.counterTutorialFinished)
                            {
                                counterTutorialStage = CounterTutorialStage::Finished;
                            }
                            else
                            {
                                counterTutorialStage = CounterTutorialStage::DragToInventory;
                            }

                            cutSound.value().play();

                            setCreatureMood(CreatureMood::Happy);
                            creatureClock.restart();

                            // START CUT ANIMATION
                            isAnimating = true;
                            animationTool = ToolType::Knife;

                            animationSprite = toolItems[0].sprite;
                            animationSprite.value().setPosition({ 300.f, 1350.f });
                            animationSprite.value().setScale({ 0.3f, 0.3f });

                            animationClock.restart();


                            currentTool = ToolType::None; //unselect the tool
                            showToolCursor = false;
                            window.setMouseCursorVisible(true);
                            std::cout << "Cut!\n";
                        }
                        else
                        {
                            manager.wrongBuzzSound.value().play();
                            std::cout << "Can't cut yet!\n";
                            currentTool = ToolType::None;

                            setCreatureMood(CreatureMood::Angry);
                            creatureClock.restart();

                            showToolCursor = false;
                            window.setMouseCursorVisible(true);
                        }
                    }
                }

                //PEEL (horizontal)
                else if (currentTool == ToolType::Peeler && dx > dy)
                {
                    if (selectedIngredient->state == IngredientState::Peeled)
                    {
                        manager.wrongBuzzSound.value().play();

                        setCreatureMood(CreatureMood::Angry);
                        creatureClock.restart();

                        std::cout << "Already peeled!\n";
                        currentTool = ToolType::None;
                        showToolCursor = false;
                        window.setMouseCursorVisible(true);
                    }
                    else if (selectedIngredient->state == IngredientState::Whole &&
                        canPeel(selectedIngredient->type))
                    {
                        selectedIngredient->state = IngredientState::Peeled;
                        selectedIngredient->updateSprite();

                        if (manager.counterTutorialFinished)
                        {
                            counterTutorialStage = CounterTutorialStage::Finished;
                        }
                        else
                        {
                            counterTutorialStage = CounterTutorialStage::Cut;
                        }
                        peelSound.value().play();

                        setCreatureMood(CreatureMood::Happy);
                        creatureClock.restart();

                        // START PEEL ANIMATION
                        isAnimating = true;
                        animationTool = ToolType::Peeler;

                        animationSprite = toolItems[1].sprite;
                        animationSprite.value().setPosition({ 300.f, 1350.f });
                        animationSprite.value().setScale({ 0.3f, 0.3f });

                        animationClock.restart();

                        currentTool = ToolType::None;//unselect the tool
                        showToolCursor = false;
                        window.setMouseCursorVisible(true);
                        std::cout << "Peeled!\n";
                    }
                    else
                    {
                        manager.wrongBuzzSound.value().play();
                        std::cout << "Can't peel this!\n";
                        currentTool = ToolType::None;

                        setCreatureMood(CreatureMood::Angry);
                        creatureClock.restart();

                        showToolCursor = false;
                        window.setMouseCursorVisible(true);
                    }
                }
            }
        }

        isDraggingGesture = false;
        selectedIngredient = nullptr;

        //from inventoy to counter

        if (cuttingBoardArea.contains(mousePos))
        {
            auto item = manager.inventory.takeDraggedItem();
            if (item)
            {
                item->isDragging = false;
                // place item at board center
                item->sprite.setPosition({ 475.f, 375.f });
                item->sprite.setScale({ 0.5f, 0.5f });

                manager.placeSound.value().play();

                setCreatureMood(CreatureMood::SideEye);
                creatureClock.restart();


                counterItems.push_back(move(item));

                if (manager.counterTutorialFinished)
                {
                    counterTutorialStage = CounterTutorialStage::Finished;
                }
                else 
                {

                    if (counterTutorialStage == CounterTutorialStage::DragToBoard)
                    {
                        counterTutorialStage = CounterTutorialStage::Peel;
                    }
                }

                return;
            }
        }

        //counter to inventory

        for (size_t i = 0; i < counterItems.size(); i++)
        {
            if (counterItems[i]->isDragging)
            {
                if (manager.inventory.contains(mousePos))
                {
                    auto item = std::move(counterItems[i]);
                    counterItems.erase(counterItems.begin() + i);

                    item->isDragging = false;
                    item->sprite.setScale({ 0.2f, 0.2f });
                    //manager.inventory.addItem(std::move(item));

                    manager.counterTutorialFinished = true;
                    counterTutorialStage = CounterTutorialStage::Finished;

                    int slotIndex = manager.inventory.getSlotIndexAt(mousePos);


                    if (slotIndex != -1)
                    {
                        manager.popSound.value().play();
                        manager.inventory.insertItemAt(std::move(item), slotIndex);
                    }
                    else
                    {
                        // fallback: put in first available slot
                        manager.popSound.value().play();
                        manager.inventory.addItem(std::move(item));
                    }
                }
                else
                {
                    // snap back if not dropped in inventory
                    counterItems[i]->isDragging = false;
                }
                return;
            }
        }

        // Reset!
        //selectedIngredient = nullptr;

        mouseHeld = false;
        pendingDragItem = nullptr;
    }
}  

void KitchenCounterState::update()
{
    if (creatureMood != CreatureMood::Default &&
        creatureClock.getElapsedTime().asSeconds() > creatureDuration)
    {
        setCreatureMood(CreatureMood::Default);
    }

    hintAnimTime += 0.003f;

    if (counterTutorialStage == CounterTutorialStage::DragToBoard)
    {
        float t = (std::sin(hintAnimTime) + 1.f) * 0.5f;

        sf::Vector2f start{ 70.f,100.f };
        sf::Vector2f end{ 540.f,480.f };

        tutorialArrow.setPosition(
            {
                start.x + (end.x - start.x) * t,
                start.y + (end.y - start.y) * t
            });
    }

    if (counterTutorialStage == CounterTutorialStage::Peel)
    {
        peelLine.setPosition({ 320.f,270.f });

        float t = (std::sin(hintAnimTime) + 1.f) * 0.5f;

        peelerFeedback.setPosition(
            {
                360.f + t * 320.f,
                290.f
            });
    }

    if (counterTutorialStage == CounterTutorialStage::Cut)
    {
        cutLine.setPosition({ 320.f,550.f });

        float t = (std::sin(hintAnimTime) + 1.f) * 0.5f;

        knifeFeedback.setPosition(
            {
                500.f - t * 200.f,
                280.f + t * 200.f
            });
    }

    if (counterTutorialStage == CounterTutorialStage::DragToInventory)
    {
        float t = (std::sin(hintAnimTime) + 1.f) * 0.5f;

        sf::Vector2f start{ 540.f,430.f };
        sf::Vector2f end{ 70.f,100.f };

        tutorialArrow.setPosition(
            {
                start.x + (end.x - start.x) * t,
                start.y + (end.y - start.y) * t
            });
    }

    manager.inventory.update();

   // sf::Vector2f mousePos = manager.inventory.getMousePos(); 

    for (auto& ing : counterItems)
    {
        if (ing->isDragging)
        {
            ing->sprite.setPosition(currentMousePos + ing->dragOffset);
        }
    }

    if (isAnimating)
    {
        float t = animationClock.getElapsedTime().asSeconds();

        // animation finished
        if (t >= animationDuration)
        {
            isAnimating = false;
        }
        else
        {
            // KNIFE animation
            if (animationTool == ToolType::Knife)
            {
                float offset = std::sin(t * 40.f) * 20.f;

                animationSprite.value().setPosition({
                        380.f,
                        250.f + offset
                    });
            }

            // PEELER animation
            else if (animationTool == ToolType::Peeler)
            {
                float offset = std::sin(t * 20.f) * 30.f;


                animationSprite.value().setPosition({
                        380.f,
                        250.f + offset
                    });
            }
        }
    }
 
}

void KitchenCounterState::draw(sf::RenderWindow& window)
{
    window.draw(background);

    window.draw(creature);

    window.draw(cookbookButton);
    window.draw(cookbookText);


    // Inventory bar <= top inventory
    manager.inventory.draw(window);

    

    // Cutting board ingredients
    for (auto& ing : counterItems)
    {
        window.draw(ing->sprite);
    }

    // tools

    for (auto& tool : toolItems)
    {
        window.draw(tool.sprite);
    }


    //animation
    if (isAnimating && animationSprite.has_value())
    {
        window.draw(animationSprite.value());
    }

    if (showToolCursor)
    {
        window.draw(toolCursor);
    }

    switch (counterTutorialStage)
    {
    case CounterTutorialStage::DragToBoard:
        window.draw(tutorialArrow);
        break;

    case CounterTutorialStage::Peel:
        window.draw(peelLine);
        window.draw(peelerFeedback);
        break;

    case CounterTutorialStage::Cut:
        window.draw(cutLine);
        window.draw(knifeFeedback);
        break;

    case CounterTutorialStage::DragToInventory:
        window.draw(tutorialArrow);
        break;

    default:
        break;
    }


 

    /*    sf::RectangleShape debug;
    debug.setPosition( cuttingBoardArea.position);
    debug.setSize( cuttingBoardArea.size);
    debug.setFillColor(sf::Color(255, 0, 0, 80));

    window.draw(debug);*/
    // debug

}

void KitchenCounterState::setCreatureMood(CreatureMood mood)
{

    std::cout << "Changing mood...\n";


    creatureMood = mood;

    switch (mood)
    {
    case CreatureMood::Default:
        creature.setTexture(&manager.creatureDefaultTexture);
        break;

    case CreatureMood::Happy:
        creature.setTexture(&manager.creatureHappyTexture);
        break;

    case CreatureMood::Angry:
        creature.setTexture(&manager.creatureAngryTexture);
        break;

    case CreatureMood::Scared:
        creature.setTexture(&manager.creatureScaredTexture);
        break;

    case CreatureMood::SideEye:
        creature.setTexture(&manager.creatureSideEyeTexture);
        break;
    }

    std::cout << "Mood changed.\n";
}

