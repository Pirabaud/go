#include "BoardScene.hpp"

#include "DisplayService.hpp"
#include "SFML/Graphics/CircleShape.hpp"

float BoardScene::PADDING = 40.0f;

BoardScene::BoardScene(sf::RenderWindow& window)
{
    backgroundColor = sf::Color(206, 163, 70);
}

void BoardScene::draw(sf::RenderWindow& window)
{
    window.clear(backgroundColor);
    drawBoard(window);
    drawStones(window);
    window.display();
}

void BoardScene::update()
{
}

void BoardScene::drawBoard(sf::RenderWindow& window)
{
    const float boardPixelSize = DisplayService::WINDOW_HEIGHT - 2 * PADDING; // 200 is the width of the sidebar
    const float cellSize = boardPixelSize / (Board::SIZE - 1);

    sf::Vertex line[2];

    for (int i = 0; i < Board::SIZE; ++i)
    {
        const float y = PADDING + i * cellSize;
        line[0] = sf::Vertex{sf::Vector2f(PADDING, y), sf::Color::Black};
        line[1] = sf::Vertex{sf::Vector2f(PADDING + cellSize * (Board::SIZE - 1), y), sf::Color::Black};
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }

    for (int i = 0; i < Board::SIZE; ++i)
    {
        const float x = PADDING + i * cellSize;
        line[0] = sf::Vertex{sf::Vector2f(x, PADDING), sf::Color::Black};
        line[1] = sf::Vertex{sf::Vector2f(x, PADDING + cellSize * (Board::SIZE - 1)), sf::Color::Black};
        window.draw(line, 2, sf::PrimitiveType::Lines);
    }
}

void BoardScene::drawStones(sf::RenderWindow& window)
{
    drawSingleColorStone(board.getGridWhite(), window, sf::Color::White);
    drawSingleColorStone(board.getGridBlack(), window, sf::Color::Black);
}

void BoardScene::drawSingleColorStone(const Board::StoneMask& stonesMask, sf::RenderWindow& window,
                                      const sf::Color& color)
{
    const float boardPixelSize = DisplayService::WINDOW_HEIGHT - 2 * PADDING; // 200 is the width of the sidebar
    const float cellSize = boardPixelSize / (Board::SIZE - 1);
    for (int row = 0; row < Board::SIZE; ++row)
    {
        for (int col = 0; col < Board::SIZE; ++col)
        {
            if (stonesMask[row] & (1 << col))
            {
                // Draw a stone at the position
                const float x = PADDING + (Board::SIZE - col - 1) * cellSize;
                const float y = PADDING + row * cellSize;
                sf::CircleShape stone(cellSize / 2.0f - 2.0f);
                stone.setFillColor(color);
                stone.setPosition({x - stone.getRadius(), y - stone.getRadius()});
                window.draw(stone);
            }
        }
    }
}

bool BoardScene::handleStonePlacement(const std::optional<sf::Event>& event, sf::RenderWindow& window)
{
    if (!event.has_value()) return false;

    if (const auto& mousePressedEvent = event->getIf<sf::Event::MouseButtonPressed>(); mousePressedEvent &&
        mousePressedEvent->button == sf::Mouse::Button::Left)
    {
        const float boardPixelSize = DisplayService::WINDOW_HEIGHT - 2 * PADDING;
        const float cellSize = boardPixelSize / (Board::SIZE - 1);
        const float circleRadius = cellSize / 2.0f - 2.0f;

        const auto& mousePos = mousePressedEvent->position;
        const float fx = static_cast<float>(mousePos.x);
        const float fy = static_cast<float>(mousePos.y);

        for (int row = 0; row < Board::SIZE; ++row)
        {
            for (int col = 0; col < Board::SIZE; ++col)
            {
                const float cx = PADDING + col * cellSize;
                const float cy = PADDING + row * cellSize;

                const float dx = fx - cx;
                const float dy = fy - cy;
                const float distSq = dx * dx + dy * dy;

                if (distSq <= circleRadius * circleRadius)
                {
                    playMove(row, col);
                    return true;;
                }
            }
        }
    }
    return false;
}

void BoardScene::playMove(const int& row, const int& col)
{
    if (colorToPlay == sf::Color::White)
    {
        board.addStoneWhite(row, col);
    }
    else
    {
        board.addStoneBlack(row, col);
    }
    nextTurn();
}

void BoardScene::nextTurn()
{
    colorToPlay == sf::Color::White ? colorToPlay = sf::Color::Black : colorToPlay = sf::Color::White;
}
