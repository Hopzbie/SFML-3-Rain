// library
#include <SFML/Graphics.hpp>
#include <random>

// structure : drop
struct Drop
{
    // state
    bool active = false;

    // position
    float x = 0.f;
    float y = 0.f;

    // scale
    float s = 1.f;
};

// main program
int main()
{
    // drops
    constexpr int n = 200;
    Drop drops[n];

    // current drop
    int current = 0;

    // shape
    float w = 1.f;
    float h = 50.f;
    sf::RectangleShape shape({w, h});
    shape.setOrigin({0.f, h});
    shape.setFillColor({255, 255, 255, 64});

    // speed
    float speed = 1500.f;

    // countdown timer
    float duration = 0.05f;
    float timer    = 0.f;

    // distance between spawns
    float distance = speed * duration;

    // create window
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Title", sf::State::Windowed, settings);

    // random number generator
    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution(0, window.getSize().x - w);

    // frame rate limit
    float fps_max = 60.f;
    float dt_min  = 1.f / fps_max;
    sf::Clock clock;

    // window loop
    while (window.isOpen())
    {
        // delta time
        float dt = clock.getElapsedTime().asSeconds();
        if (dt >= dt_min)
        {
            clock.restart();

            // handle events
            while (const auto event = window.pollEvent())
            {
                // on window close
                if (event->is<sf::Event::Closed>())
                {
                    // close window
                    window.close();
                }

                // on window resize
                else if (const auto* resized = event->getIf<sf::Event::Resized>())
                {
                    // update view
                    window.setView(sf::View(sf::FloatRect({0.f, 0.f}, sf::Vector2f(resized->size))));
                
                    // update distribution
                    distribution = std::uniform_int_distribution<int>(0, window.getSize().x - w);
                }
            }

            // update timer
            timer -= dt;
            if (timer <= 0.f)
            {
                // reset timer
                timer = duration;

                // spawn drops
                int n_spawn = 10 * window.getSize().x / 800;
                for (int i = 0;  i < n_spawn;  i++)
                {
                    // spawn a drop
                    float x = distribution(generator);
                    float y = -distance * i / n_spawn;
                    float s = (current % 2) ? 1.f : 0.5f;
                    drops[current] = {true, x, y, s};
                
                    // next drop
                    current = (current + 1) % n;
                }
            }

            // update drops
            for (auto& drop : drops)
            {
                // skip inactive
                if (not drop.active)
                    continue;

                // move
                drop.y += speed * dt;

                // inactive when out of window
                if (drop.y - h*drop.s > window.getSize().y)
                    drop.active = false;
            }

            // fill window with color
            window.clear({64, 64, 64});

            // draw drops
            for (auto& drop : drops)
            {
                // skip inactive
                if (not drop.active)
                    continue;

                shape.setPosition({drop.x, drop.y});
                shape.setScale({drop.s, drop.s});
                window.draw(shape);
            }

            // display current frame
            window.display();
        }
    }

    // program end successfully
    return 0;
}