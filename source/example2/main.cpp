#include <ceres/ceres.h>
#include <glog/logging.h>
#include <Eigen/Core>
#include <random>


// Minimize this
struct BlackBoxCostFunctor
{
    //! \param[in] p point or any coefficients
    //! \param[out] cost Positive cost to minimize
    //! \retval true for ok, I guess
    bool operator()(const double* const p, double* cost) const
    {
        const Eigen::Vector3d target{ -2.0, 3.0, 42.0 };
        const Eigen::Vector3d position{ p[0], p[1], p[2] };
        *cost = 1.0 + (target - position).squaredNorm();    // Parabola converges in ~50 iterations - faster with +1, but residual must not be negative
        //*cost = 1.0 + (target - position).norm();           // This V-shape converges in 4 iterations
        return true;
    }
};


int main(int, char** argv)
{
    google::InitGoogleLogging(argv[0]);

    double position[3] = { 0.0, 0.0, 0.0 };
    ceres::Problem problem;
    // dimension of residual (1) and value(3)
    auto* cost_fun = new ceres::NumericDiffCostFunction<BlackBoxCostFunctor, ceres::CENTRAL, 1, 3>(new BlackBoxCostFunctor());
    problem.AddResidualBlock(cost_fun, nullptr, position);

    ceres::Solver::Options options;
    options.minimizer_progress_to_stdout = true;
    options.max_num_iterations = 250;
    options.minimizer_type = ceres::LINE_SEARCH; // iterates much faster
    //options.minimizer_type = ceres::TRUST_REGION; // does not work

    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);
    std::cout << summary.BriefReport() << "\n";
    std::cout << "Final solution: \n    " << Eigen::Vector3d(position);

    return 0;
}
