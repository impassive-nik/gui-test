#ifndef __INCLUDE_NATIVE_WINDOW_H__
#define __INCLUDE_NATIVE_WINDOW_H__

#include "imgui.h"
#include "ResourcesManager.h"

class NativeWindow {
public:
	int x, y;
	int width, height;
	const char *name;
	ImVec4 bg_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	NativeWindow(const char* name, int x = 10, int y = 10, int width = 1200, int height = 900) : x(x), y(y), width(width), height(height), name(name) {}
	virtual void run() = 0;
	virtual ~NativeWindow() {}
};

class MyWindow : public NativeWindow {
	ResManager *res_mgr;
public:
	MyWindow(ResManager* res_mgr = nullptr);
	void run() override;
};

#endif // __INCLUDE_NATIVE_WINDOW_H__