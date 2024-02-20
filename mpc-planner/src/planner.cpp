#include <mpc-planner/planner.h>

#include <mpc-planner-modules/modules.h> /** @note: Autogenerated */

#include <mpc-planner-types/controller_module.h>
#include <mpc-planner-types/realtime_data.h>
#include <mpc-planner-solver/solver_interface.h>

#include <mpc-planner-util/load_yaml.hpp>
#include <mpc-planner-util/logging.h>
#include <mpc-planner-util/visuals.h>
#include <mpc-planner-util/parameters.h>

namespace MPCPlanner
{

    Planner::Planner()
    {
        // Initialize the solver
        _solver = std::make_shared<Solver>();
        _solver->reset();

        initializeModules(_modules, _solver);

        _benchmarker = std::make_unique<Benchmarker>("optimization");
    }

    // Given real-time data, solve the MPC problem
    PlannerOutput Planner::solveMPC(State &state, const RealTimeData &data)
    {
        _output = PlannerOutput(_solver->dt, _solver->N);

        // Check if all modules have enough data
        bool is_data_ready = true;
        std::string missing_data;
        for (auto &module : _modules)
            is_data_ready = is_data_ready & module->isDataReady(data, missing_data);

        if (!is_data_ready)
        {
            LOG_WARN("Data is not ready, missing " + missing_data + "\b");
            _output.success = false;
            return _output;
        }

        // Set the initial state
        _solver->setXinit(state);

        // Set the initial guess
        _solver->setWarmstart(state);

        // Update all modules
        for (auto &module : _modules)
            module->update(state, data);

        for (int k = 0; k < _solver->N; k++)
        {
            for (auto &module : _modules)
                module->setParameters(data, k);
        }

        // Solve MPC
        _benchmarker->start();
        int exit_flag = _solver->solve();
        _benchmarker->stop();

        if (exit_flag != 1)
        {
            _output.success = false;
            LOG_WARN("MPC did not find a solution");
            LOG_VALUE("Exit Flag", exit_flag); /** @todo: Convertion to text */
            return _output;
        }

        _output.success = true;
        for (int k = 1; k < _solver->N; k++)
            _output.trajectory.add(_solver->getOutput(k, "x"), _solver->getOutput(k, "y"));

        return _output;
    }

    double Planner::getSolution(int k, std::string &&var_name)
    {
        return _solver->getOutput(k, std::forward<std::string>(var_name));
    }

    void Planner::onDataReceived(RealTimeData &data, std::string &&data_name)
    {
        for (auto &module : _modules)
            module->onDataReceived(data, std::forward<std::string>(data_name));
    }

    void Planner::visualize(const State &state, const RealTimeData &data)
    {
        (void)state;

        for (auto &module : _modules)
            module->visualize(data);

        visualizeTrajectory(_output.trajectory, "planned_trajectory", true);

        visualizeObstacles(data.dynamic_obstacles, "obstacles", true);
        visualizeObstaclePredictions(data.dynamic_obstacles, "obstacle_predictions", true);
    }

}