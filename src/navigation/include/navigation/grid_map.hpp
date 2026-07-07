#pragma once

#include <cstdint>
#include <optional>
#include <utility>
#include <vector>


namespace navigation
{

enum class CellState : int8_t
{
    Unknown = -1,
    Free = 0,
    Occupied = 100
};

class GridMap
{
public:
    GridMap(int width, int height, double resolution, double origin_x, double origin_y);
    
    int width() const;
    int height() const;
    double resolution() const;
    double origin_x() const;
    double origin_y() const;

    CellState getCell(int cell_x, int cell_y) const;
    void setCell(int cell_x, int cell_y, CellState state);

    bool isFree(int cell_x, int cell_y) const;
    bool isOccupied(int cell_x, int cell_y) const;
    bool isTraversable(int cell_x, int cell_y) const;

    std::optional<std::pair<int, int>> worldToGrid(double x, double y) const;
    std::pair<double, double> gridToWorld(int cell_x, int cell_y) const;

    const std::vector<int8_t>& data() const;

private:
    int index(int cell_x, int cell_y) const;

    int width_{0};
    int height_{0};
    double resolution_{0.05};
    double origin_x_{0.0};
    double origin_y_{0.0};

    std::vector<int8_t> cells_; //storing the 2D map as 1D vectors is best practice in ROS

};


}