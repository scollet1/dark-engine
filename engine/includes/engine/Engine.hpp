class Engine() {
public:

	Engine();
	~Engine();

	bool    _Init();
	bool    _Run();
	bool    _Destroy();

private:
	Game                    game;
	Renderer                renderer;
	queue<t_task>           *destroy;
};