#include "Engine.h"


int main()
{
	
	sf::RenderWindow window(sf::VideoMode(800, 800), "3D", sf::Style::Default);
	sf::RenderWindow window_map(sf::VideoMode(400, 400), "3D", sf::Style::Default);
  
	v3d::Camera camera(window, sf::Vector3<float>(0, 0, 0), 60, sf::Vector2<float>(0, 0));
	camera.position.x = 125;
	camera.position.y = 50;
	camera.setZoom(3);

	v3d::Polygon polygon;

	polygon.positions.a.x = 100;
	polygon.positions.a.y = 150;
	polygon.positions.a.z = 350;
	
	polygon.positions.b.x = 150;
	polygon.positions.b.y = 150;
	polygon.positions.b.z = 350;

	polygon.positions.c.x = 100;
	polygon.positions.c.y = 100;
	polygon.positions.c.z = 50;

	polygon.positions.d.x = 150;
	polygon.positions.d.y = 100;
	polygon.positions.d.z = 50;

	v3d::Clock<float> clock;
	
	sf::RectangleShape r(sf::Vector2f(window.getSize().x / sizeX, window.getSize().y / sizeY));
	sf::RectangleShape poly(sf::Vector2f(50, 5));
	poly.setRotation(45);
	poly.setFillColor(sf::Color::Green);
	sf::CircleShape circle(16);
	circle.setFillColor(sf::Color::Blue);
	circle.setOrigin(16, 16);
	sf::RectangleShape ray(sf::Vector2f(150, 2));
	
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			camera.rotate(-1, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			camera.rotate(1, 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			camera.move(math::cosf180(camera.rotation.x), math::sinf180(camera.rotation.x), 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			camera.move(-math::cosf180(camera.rotation.x), -math::sinf180(camera.rotation.x), 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			camera.move(math::cosf180(camera.rotation.x + 90), math::sinf180(camera.rotation.x + 90), 0);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			camera.move(-math::cosf180(camera.rotation.x - 90), math::sinf180(camera.rotation.x), 0);
		}

    //////////////////DRAWING////////////////
		window.clear();
		polygon.render(camera);
		window.draw(polygon);
		window.display();
    
		window_map.clear();
		circle.setPosition(camera.position.x, camera.position.y);
		poly.setPosition(polygon.positions.a.x, polygon.positions.a.y);
		ray.setPosition(camera.position.x, camera.position.y);
		ray.setRotation(camera.rotation.x);
		window_map.draw(circle);
		window_map.draw(poly);
		window_map.draw(ray);


		window_map.display();
	}
	
	return EXIT_SUCCESS;
}
