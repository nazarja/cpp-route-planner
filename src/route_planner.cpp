#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y) : m_Model(model)
{
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // TODO 2: Use the m_Model.FindClosestNode method to find the closest nodes to the starting and ending coordinates.
    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);
}

// TODO 3: Implement the CalculateHValue method.
float RoutePlanner::CalculateHValue(RouteModel::Node const *node)
{
    return node->distance(*end_node);
}

// TODO 4: Complete the AddNeighbors method to expand the current node by adding all unvisited neighbors to the open list.
void RoutePlanner::AddNeighbors(RouteModel::Node *current_node)
{
    current_node->FindNeighbors();

    for (RouteModel::Node *node : current_node->neighbors)
    {
        node->parent = current_node;
        node->g_value = current_node->g_value + current_node->distance(*node);
        node->h_value = this->CalculateHValue(node);
        node->visited = true;
        this->open_list.push_back(node);
    }
}

bool Compare(RouteModel::Node const *node_1, RouteModel::Node const *node_2)
{
    return (node_1->g_value + node_1->h_value) > (node_2->g_value + node_2->h_value);
}

// TODO 5: Complete the NextNode method to sort the open list and return the next node.
RouteModel::Node *RoutePlanner::NextNode()
{
    // sort open list using Compare, remove node with lowest (g+h) value, return that node
    std::sort(this->open_list.begin(), this->open_list.end(), Compare);
    RouteModel::Node *node = this->open_list.back();
    this->open_list.pop_back();
    return node;
}

// TODO 6: Complete the ConstructFinalPath method to return the final path found from your A* search.
std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node)
{
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    // TODO: Implement your solution here.
    // as start node has no parent, we will be done iterating then
    while (current_node->parent != nullptr)
    {
        // push current node into path
        // increment distance to parent node
        // reassign current node to the parent for next iteration
        path_found.push_back(*current_node);
        distance += current_node->distance(*current_node->parent);
        current_node = current_node->parent;
    }
    // Multiply the distance by the scale of the map to get meters.
    distance *= m_Model.MetricScale();

    // reverse vector to have start_node at the beginning
    std::reverse(path_found.begin(), path_found.end());
    return path_found;
}

// TODO 7: Write the A* Search algorithm here.
void RoutePlanner::AStarSearch()
{
    // add start node as first node to open_list
    this->start_node->visited = true;
    this->open_list.push_back(start_node);

    // create pointer for to track current_node
    RouteModel::Node *current_node = nullptr;

    // while there are still nodes to explore
    while(this->open_list.size() > 0)
    {
        // assign current node to next node in open_list
        current_node = this->NextNode();

        // if distance to end node is 0, we are finished / at our goal
        if (current_node->distance(*this->end_node) == 0)
        {
            // save returned final path vector to m_Model.path
            this->m_Model.path = this->ConstructFinalPath(current_node);
            break;
        }
        // otherwise keep expanding neighbours
        else this->AddNeighbors(current_node);
    }
}