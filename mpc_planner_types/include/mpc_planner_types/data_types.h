#ifndef MPC_DATA_TYPES_H
#define MPC_DATA_TYPES_H

#include <Eigen/Dense>

#include <vector>

/** Basic high-level data types for motion planning */

namespace MPCPlanner
{

    struct Disc
    {
        Eigen::Vector2d position;
        double offset;

        Disc(const Eigen::Vector2d &position, double offset);
    };

    struct StaticObstacle
    {
    };

    enum class PredictionType
    {
        DETERMINISTIC = 0,
        GAUSSIAN,
        NONGAUSSIAN,
        NONE
    };

    struct PredictionStep
    {

        // Mean
        Eigen::Vector2d position;
        double angle;

        // Covariance
        double major_radius;
        double minor_radius;

        PredictionStep(const Eigen::Vector2d &position, double angle, double major_radius, double minor_radius);
    };

    typedef std::vector<PredictionStep> Mode;

    struct Prediction
    {

        PredictionType type;

        std::vector<Mode> modes;
        std::vector<double> probabilities;

        Prediction();
        Prediction(PredictionType type);

        bool empty() const;
    };

    struct DynamicObstacle
    {
        int index;

        Eigen::Vector2d position;
        double angle;

        double radius;

        Prediction prediction;

        DynamicObstacle(int _index, const Eigen::Vector2d &_position, double _angle, double _radius);
    };

    struct ReferencePath
    {

        std::vector<double> x;
        std::vector<double> y;
        std::vector<double> psi;

        ReferencePath(int length = 10);
        void clear();

        bool pointInPath(int point_num, double other_x, double other_y) const;

        // friend std::ostream &operator<<(std::ostream &out, const ReferencePath &path);
    };

    struct Trajectory
    {
        double dt;
        std::vector<Eigen::Vector2d> positions;

        Trajectory(double dt = 0., int length = 10);

        void add(const Eigen::Vector2d &p);
        void add(const double x, const double y);
    };
}

#endif