#include "block.h"
#include <cmath>
#include <iostream>

int Block::height() const {
  if (data.size() == 0) return 0;
  return data[0].size();
}

int Block::width() const {
  return data.size();
}

void Block::render(PNG &im, int x) const {
  HSLAPixel* p;
  for (int i = 0; i < width(); i++) {
    for (int j = 0; j < height(); j++) {
      p = im.getPixel(i+x, j);
      *p = data[i][j];
    }
  }
}

void Block::build(PNG &im, int x, int width) {
  HSLAPixel* p;
  HSLAPixel* np;
  for (int i = 0; i < width; i++) {
    data.push_back(vector<HSLAPixel>());
    for (unsigned int j = 0; j < im.height(); j++) {
      p = im.getPixel(i+x, j);
      np = new HSLAPixel(p->h, p->s, p->l, p->a);
      data[i].push_back(*np);
      delete np;
    }
  }
}