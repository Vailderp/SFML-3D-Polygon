template<typename T>
	class Quad
	{
	public:
		T a;
		T b;
		T c;
		T d;

		Quad() :
			a(T()),
			b(T()),
			c(T()),
			d(T())
		{

		}

		Quad(const T a, const T b, const T c, const T d) :
			a(a),
			b(b),
			c(c),
			d(d)
		{

		}
	};
  
  //////////////////CAMERA///////////////////////
  
  class Camera
	{
	private:
		void window_init(const sf::Vector2<unsigned> window_size)
		{
			this->window_size.x = window_size.x;
			this->window_size.y = window_size.y;
			this->window_size_2.x = static_cast<float>(window_size.x) / 2.f;
			this->window_size_2.y = static_cast<float>(window_size.y) / 2.f;
		}
		void change_render_constant()
		{
			this->render_constant = static_cast<float>(this->window_size.x) / abs(2 * (tanf(this->FOV) * math::PI_180))
			* static_cast<float>(this->window_size.y) * this->zoom;
		}
	public:
		//FOV - Field Of View
		sf::Vector2<float> window_size;
		sf::Vector2<float> window_size_2;
		sf::Vector3<float> position;
		sf::Vector2<float> rotation;
		float FOV;
		float zoom;
		float render_constant{};
		float WWF;


		explicit Camera(const sf::RenderWindow& window) :
			window_size(window.getSize()),
			window_size_2(sf::Vector2<int>(window.getSize().x / 2, window.getSize().y / 2)),
			position(sf::Vector3<float>(0, 0, 0)),
			rotation(sf::Vector2<float>(0, 0)),
			FOV(60),
			zoom(1),
			WWF(window_size.x / FOV)
		{
			change_render_constant();
		}
		
		Camera(const sf::RenderWindow& window, const sf::Vector3<float> position, const float FOV, 
			const sf::Vector2<float> rotation, const float zoom = 4) :
			window_size(window.getSize()),
			window_size_2(sf::Vector2<int>(window.getSize().x / 2, window.getSize().y / 2)),
			position(position),
			rotation(rotation),
			FOV(FOV),
			zoom(zoom),
			WWF(window_size.x / FOV)
		{
			change_render_constant();
		}

		void setWindow(const sf::RenderWindow& window)
		{
			window_init(window.getSize());
			WWF = window_size.x / FOV;
			change_render_constant();
		}

		void setZoom(const float zoom)
		{
			this->zoom = zoom;
			change_render_constant();
		}
		
		void rotate(const float x, const float y)
		{
			rotation.x += x;
			rotation.y += y;
			math::delimit_angle(rotation.x);
			math::delimit_angle(rotation.y);
		}

		void rotate(const sf::Vector2<float> rotation)
		{
			rotate(rotation.x, rotation.y);
		}

		void move(const float x, const float y, const float z)
		{
			position.x += x;
			position.y += y;
			position.z += z;
		}

		void move(const sf::Vector3<float> position)
		{
			move(position.x, position.y, position.z);
		}
		
		void FOV_unary(const float FOV)
		{
			this->FOV += FOV;
			WWF = window_size.x / FOV;
			change_render_constant();
		}

		void zoom_unary(const float zoom)
		{
			this->zoom += zoom;
			change_render_constant();
		}
	};
  
  
  ////////////////////////POLYGON//////////////////////////

class Polygon : public sf::Drawable, public sf::Transformable
	{
	private:
		sf::ConvexShape polygon_;
		Quad<float> distances_x_ = Quad<float>();
		Quad<float> distances_y_ = Quad<float>();
		Quad<float> distances_ = Quad<float>();
		Quad<float> angles_ = Quad<float>();
		Quad<float> signed_angles_ = Quad<float>();
		Quad<sf::Vector2<float>> screen_positions_ = Quad<sf::Vector2<float>>();

		void init()
		{
			polygon_.setPointCount(4);
			polygon_.setOutlineColor(sf::Color::Red);
			polygon_.setOutlineThickness(3);
		}

	public:
		Polygon()
		{
			init();
		}

		explicit Polygon(const Quad<sf::Vector3<float>> positions) :
			positions(positions)
		{
			init();
		}

		virtual ~Polygon() {}

		Quad<sf::Vector3<float>> positions = Quad<sf::Vector3<float>>();

		void setFillColor(const sf::Color color)
		{
			polygon_.setFillColor(color);
		}
		
		void render(const class Camera& camera)
		{

			///
			///  a___________b
			///   |			|
			///   |			|
			///	  |			|
			///  c|_________|d
			///

			//Нахождение позиции углов полилигона в двумерном пространстве

			distances_x_.a = positions.a.x - camera.position.x;
			distances_x_.b = positions.b.x - camera.position.x;
			distances_x_.c = positions.c.x - camera.position.x;
			distances_x_.d = positions.d.x - camera.position.x;

			distances_y_.a = positions.a.y - camera.position.y;
			distances_y_.b = positions.b.y - camera.position.y;
			distances_y_.c = positions.c.y - camera.position.y;
			distances_y_.d = positions.d.y - camera.position.y;

			distances_.a = sqrtf(distances_x_.a * distances_x_.a + distances_y_.a * distances_y_.a);
			distances_.b = sqrtf(distances_x_.b * distances_x_.b + distances_y_.b * distances_y_.b);
			distances_.c = sqrtf(distances_x_.c * distances_x_.c + distances_y_.c * distances_y_.c);
			distances_.d = sqrtf(distances_x_.d * distances_x_.d + distances_y_.d * distances_y_.d);

			//Нахожнеие глобального угла между камерой и углом полигона
			angles_.a = acosf(distances_x_.a / distances_.a) * (camera.position.y < positions.a.x ? 1 : -1);
			angles_.b = acosf(distances_x_.b / distances_.b) * (camera.position.y < positions.b.x ? 1 : -1);
			angles_.c = acosf(distances_x_.c / distances_.c) * (camera.position.y < positions.c.x ? 1 : -1);
			angles_.d = acosf(distances_x_.d / distances_.d) * (camera.position.y < positions.d.x ? 1 : -1);

			//разница мужду углом камеры и углом полигона
			signed_angles_.a = math::getSignedAngle(camera.rotation.x, math::toDeg(angles_.a));
			signed_angles_.b = math::getSignedAngle(camera.rotation.x, math::toDeg(angles_.b));
			signed_angles_.c = math::getSignedAngle(camera.rotation.x, math::toDeg(angles_.c));
			signed_angles_.d = math::getSignedAngle(camera.rotation.x, math::toDeg(angles_.d));

			//Позиция угла полигона на экране по координате Х
			screen_positions_.a.y = (camera.window_size_2.y + camera.render_constant / distances_.a / 2.f) - 
				positions.a.z * (camera.render_constant / distances_.a * 0.002f);
			screen_positions_.b.y = (camera.window_size_2.y + camera.render_constant / distances_.b / 2.f) -
				positions.b.z * (camera.render_constant / distances_.b * 0.002f);
			screen_positions_.c.y = (camera.window_size_2.y + camera.render_constant / distances_.c / 2.f) -
				positions.c.z * (camera.render_constant / distances_.c * 0.002f);
			screen_positions_.d.y = (camera.window_size_2.y + camera.render_constant / distances_.d / 2.f) -
				positions.d.z * (camera.render_constant / distances_.d * 0.002f);

			//Позиция угла полигона на экране по координате У
			screen_positions_.a.x = camera.window_size_2.x + signed_angles_.a * camera.WWF;
			screen_positions_.b.x = camera.window_size_2.x + signed_angles_.b * camera.WWF;
			screen_positions_.c.x = camera.window_size_2.x + signed_angles_.c * camera.WWF;
			screen_positions_.d.x = camera.window_size_2.x + signed_angles_.d * camera.WWF;

			//Отображение полигона с помощью SFML
			polygon_.setPoint(0, screen_positions_.a);
			polygon_.setPoint(1, screen_positions_.b);
			polygon_.setPoint(3, screen_positions_.c);
			polygon_.setPoint(2, screen_positions_.d);
		}

	protected:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			if (abs(signed_angles_.a - signed_angles_.b) < 180)
			{
				target.draw(polygon_);
			}
		}
	};
