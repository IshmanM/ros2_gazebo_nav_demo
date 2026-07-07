#include "navigation/grid_map.hpp"

#include <cmath>
#include <stdexcept>

namespace navigation
{

GridMap::GridMap(
    int width,
    int height,
    double resolution,
    double origin_x,
    double origin_y
): 
  width_(width),
  height_(height),
  resolution_(resolution),
  origin_x_(origin_x),
  origin_y_(origin_y),
  cells_(width * height, static_cast<int8_t>(CellState::Free))
{
    if (width <= 0) {
        throw std::invalid_argument("GridMap width must be positive");
    }

    if (height <= 0) {
        throw std::invalid_argument("GridMap height must be positive");
    }

    if (resolution <= 0.0) {
        throw std::invalid_argument("GridMap resolution must be positive");
    }
}

int GridMap::width() const
{
    return width_;
}

int GridMap::height() const
{
    return height_;
}

double GridMap::resolution() const
{
    return resolution_;
}

double GridMap::origin_x() const
{
    return origin_x_;
}

double GridMap::origin_y() const
{
    return origin_y_;
}

CellState GridMap::getCell(int cell_x, int cell_y) const
{
    if (
        cell_x < 0 || cell_x >= width_ ||
        cell_y < 0 || cell_y >= height_
    ) {
        throw std::out_of_range("GridMap::getCell cell is out of bounds");
    }

    return static_cast<CellState>(cells_[index(cell_x, cell_y)]);
}

void GridMap::setCell(int cell_x, int cell_y, CellState state)
{
    if (
        cell_x < 0 || cell_x >= width_ ||
        cell_y < 0 || cell_y >= height_
    ) {
        throw std::out_of_range("GridMap::setCell cell is out of bounds");
    }

    cells_[index(cell_x, cell_y)] = static_cast<int8_t>(state);
}

bool GridMap::isFree(int cell_x, int cell_y) const
{
    if (
        cell_x < 0 || cell_x >= width_ ||
        cell_y < 0 || cell_y >= height_
    ) {
        return false;
    }

    return getCell(cell_x, cell_y) == CellState::Free;
}

bool GridMap::isOccupied(int cell_x, int cell_y) const
{
    if (
        cell_x < 0 || cell_x >= width_ ||
        cell_y < 0 || cell_y >= height_
    ) {
        return true;
    }

    return getCell(cell_x, cell_y) == CellState::Occupied;
}

bool GridMap::isTraversable(int cell_x, int cell_y) const
{
    if (
        cell_x < 0 || cell_x >= width_ ||
        cell_y < 0 || cell_y >= height_
    ) {
        return false;
    }

    // Traversability rules may change in future...

    return getCell(cell_x, cell_y) == CellState::Free;
}

std::optional<std::pair<int, int>> GridMap::worldToGrid(double x, double y) const
{
    const int cell_x = static_cast<int>(
        std::floor((x - origin_x_) / resolution_)
    );

    const int cell_y = static_cast<int>(
        std::floor((y - origin_y_) / resolution_)
    );

    if (
        cell_x < 0 || cell_x >= width_ ||
        cell_y < 0 || cell_y >= height_
    ) {
        return std::nullopt;
    }

    return std::make_pair(cell_x, cell_y);
}

std::pair<double, double> GridMap::gridToWorld(int cell_x, int cell_y) const
{
    if (
        cell_x < 0 || cell_x >= width_ ||
        cell_y < 0 || cell_y >= height_
    ) {
        throw std::out_of_range("GridMap::gridToWorld cell is out of bounds");
    }

    const double x = origin_x_ + (static_cast<double>(cell_x) + 0.5) * resolution_;
    const double y = origin_y_ + (static_cast<double>(cell_y) + 0.5) * resolution_;

    return std::make_pair(x, y);
}

const std::vector<int8_t>& GridMap::data() const
{
    return cells_;
}

int GridMap::index(int cell_x, int cell_y) const
{
    return cell_y * width_ + cell_x;
}


}