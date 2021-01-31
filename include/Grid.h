#ifndef __INCLUDE_NATIVE_GRID_H__
#define __INCLUDE_NATIVE_GRID_H__

#include <memory>

template <typename T> class Grid {
  int w = 0;
  int h = 0;
  T *data = nullptr;

public:
  Grid(int width = 0, int height = 0, const T &value = T()) : w(width), h(height) {
    if (width <= 0 || height <= 0) {
      w = 0;
      h = 0;
      return;
    }
    int size = w * h;
    data = new T[size];
    for (int i = 0; i < size; ++i)
      data[i] = value; // TODO: fill?
  }

  bool empty() const { return !data; }

  int getWidth() const { return w; }
  int getHeight() const { return h; }

  const T &at(int x, int y) const { return data[x + y * w]; }
  T       &at(int x, int y)       { return data[x + y * w]; }

  bool isValid(int x, int y) const { return x >= 0 && x < w && y >= 0 && y < h; }

  ~Grid() {
    if (data)
      delete[] data;
  }
};

#endif // __INCLUDE_NATIVE_GRID_H__