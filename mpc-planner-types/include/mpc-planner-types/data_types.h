#ifndef MPC_DATA_TYPES_H
#define MPC_DATA_TYPES_H

#include <Eigen/Dense>

/** Basic high-level data types for motion planning */

namespace MPCPlanner
{

    struct DynamicObstacle
    {
        int index;
        Eigen::Vector2d position;
        Eigen::Vector2d velocity;

        DynamicObstacle(int _index, const Eigen::Vector2d &_position, const Eigen::Vector2d &_velocity)
            : index(_index), position(_position), velocity(_velocity)
        {
        }
    };

    struct StaticObstacle
    {
    };

    struct ReferencePath
    {

        std::vector<double> x;
        std::vector<double> y;
        std::vector<double> psi;

        ReferencePath(int length = 10)
        {
            x.reserve(length);
            y.reserve(length);
            psi.reserve(length);
        };

        // void addPose(const geometry_msgs::msg::Pose &pose);
        // void addPoint(const geometry_msgs::msg::Point &point);
        void clear();

        friend std::ostream &operator<<(std::ostream &out, const ReferencePath &path)
        {
            out << "Path:\n";
            for (size_t i = 0; i < path.x.size(); i++)
            {
                out << "(" << path.x[i] << ", " << path.y[i] << ", " << path.psi[i] << ")" << std::endl;
            }
            return out;
        }
    };
};

#endif