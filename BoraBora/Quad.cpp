#pragma once

#include "Quad.h"

Quad::Quad() {}

Quad::Quad(sf::Vertex v0, sf::Vertex v1, sf::Vertex v2, sf::Vertex v3) : v0(v0), v1(v1), v2(v2), v3(v3) {
}

Quad::~Quad() {
}
