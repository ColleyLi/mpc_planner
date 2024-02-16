/** This file was autogenerated by the mpc-planner-solver package at 08:47AM on February 12, 2024*/
#ifndef __MODEL_H__
#define __MODEL_H__

#include <mpc-planner-solver/state.h>

#include <mpc-planner-util/load_yaml.hpp>
// #include <vector>
// #include <iostream>
#include <memory>

#include <Solver.h>
#include <Solver_memory.h>

extern "C"
{
	extern solver_int32_default Solver_adtool2forces(Solver_float *x,				 /* primal vars                                         */
													 Solver_float *y,				 /* eq. constraint multiplers                           */
													 Solver_float *l,				 /* ineq. constraint multipliers                        */
													 Solver_float *p,				 /* parameters                                          */
													 Solver_float *f,				 /* objective function (scalar)                         */
													 Solver_float *nabla_f,			 /* gradient of objective function                      */
													 Solver_float *c,				 /* dynamics                                            */
													 Solver_float *nabla_c,			 /* Jacobian of the dynamics (column major)             */
													 Solver_float *h,				 /* inequality constraints                              */
													 Solver_float *nabla_h,			 /* Jacobian of inequality constraints (column major)   */
													 Solver_float *hess,			 /* Hessian (column major)                              */
													 solver_int32_default stage,	 /* stage number (0 indexed)                            */
													 solver_int32_default iteration, /* iteration number of solver                          */
													 solver_int32_default threadID /* Id of caller thread 								   */);
}

namespace MPCPlanner
{
	class Solver
	{

	protected:
		State _state;

		char *_solver_memory;
		Solver_mem *_solver_memory_handle;

	public:
		int _solver_id;

		Solver_params _params;
		Solver_output _output;
		Solver_info _info;

		int N;			   // Horizon length
		unsigned int nu;   // Number of control variables
		unsigned int nx;   // Differentiable variables
		unsigned int nvar; // Total variable count
		unsigned int npar; // Parameters per iteration
		double dt;

		YAML::Node _config, _parameter_map, _model_map;

		Solver(int solver_id = 0);
		void reset();
		~Solver();

		void setVar(unsigned int k, std::string &&var_name, double value);
		double getVar(unsigned int k, std::string &&var_name);

		/** @brief Set and get a solver parameter at index index of stage k */
		void setParameter(int k, std::string &&parameter, double value);
		double getParameter(int k, std::string &&parameter);
		void setXinit(std::string &&state_name, double value);
		void setXinit(const State &state);

		/** @brief Solve the optimization */
		int solve();
		double getOutput(int k, std::string &&state_name);
	};
}
#endif