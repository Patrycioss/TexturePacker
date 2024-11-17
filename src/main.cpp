#include <app.hpp>

int main() {
	App app{1000, 800, {0.45f, 0.55f, 0.60f, 1.0f}};
	app.start();
	return 0;
}