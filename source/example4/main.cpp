#include "simplex.h"
#include <array>
#include <utility>
#include <chrono>

#if 0
// Exact points
static const std::array<std::pair<double, double>, 51> timeline = { {
    { 0.000000, 3.000000 },    { 0.280914, 2.085697 },    { 0.406823, 1.808335 },    { 0.610224, 1.475305 },    { 0.804635, 1.251422 },
    { 0.968138, 1.112445 },    { 1.367117, 0.890658 },    { 1.483477, 0.846190 },    { 1.577503, 0.814700 },    { 1.779516, 0.757473 },
    { 2.004334, 0.706054 },    { 2.219897, 0.665075 },    { 2.395670, 0.635922 },    { 2.531239, 0.615413 },    { 2.746141, 0.585632 },
    { 2.868844, 0.569843 },    { 3.228088, 0.527480 },    { 3.428818, 0.505808 },    { 3.600790, 0.488166 },    { 3.832830, 0.465543 },
    { 3.994762, 0.450478 },    { 4.257604, 0.427166 },    { 4.436478, 0.412048 },    { 4.535444, 0.403928 },    { 4.855385, 0.378796 },
    { 5.043617, 0.364767 },    { 5.161904, 0.356224 },    { 5.331822, 0.344305 },    { 5.615171, 0.325318 },    { 5.875870, 0.308781 },
    { 6.064621, 0.297337 },    { 6.160544, 0.291686 },    { 6.436834, 0.276002 },    { 6.550626, 0.269790 },    { 6.742574, 0.259628 },
    { 7.040677, 0.244600 },    { 7.229635, 0.235529 },    { 7.414085, 0.226998 },    { 7.599097, 0.218752 },    { 7.803813, 0.209976 },
    { 8.016282, 0.201240 },    { 8.172115, 0.195065 },    { 8.390634, 0.186724 },    { 8.628149, 0.178061 },    { 8.759707, 0.173437 },
    { 9.000974, 0.165267 },    { 9.266644, 0.156715 },    { 9.419101, 0.152008 },    { 9.570235, 0.147482 },    { 9.806079, 0.140687 },
    { 9.945036, 0.136831 },
} };
#else
// Noisy data
const std::array<std::pair<double, double>, 51> timeline = { {
    { 0.000000, 3.189510 },    { 0.159339, 2.586585 },    { 0.436577, 1.848113 },    { 0.634056, 1.576978 },    { 0.752158, 1.373816 },
    { 1.026225, 1.232527 },    { 1.230052, 0.821631 },    { 1.453301, 0.825315 },    { 1.601001, 0.732993 },    { 1.755498, 0.703383 },
    { 1.996400, 0.669080 },    { 2.160312, 0.781661 },    { 2.338352, 0.589122 },    { 2.615413, 0.649779 },    { 2.726784, 0.583808 },
    { 2.945299, 0.599699 },    { 3.101722, 0.625381 },    { 3.339620, 0.499541 },    { 3.662687, 0.491819 },    { 3.770834, 0.434225 },
    { 4.055200, 0.448456 },    { 4.152308, 0.403228 },    { 4.483644, 0.407655 },    { 4.560728, 0.442735 },    { 4.797161, 0.350470 },
    { 5.003867, 0.370958 },    { 5.218464, 0.333265 },    { 5.400906, 0.296857 },    { 5.588184, 0.304204 },    { 5.806071, 0.244432 },
    { 5.969361, 0.319830 },    { 6.254378, 0.254922 },    { 6.375601, 0.313930 },    { 6.590981, 0.308705 },    { 6.779105, 0.221304 },
    { 7.059069, 0.222936 },    { 7.139712, 0.216437 },    { 7.333438, 0.235179 },    { 7.594796, 0.215476 },    { 7.773332, 0.207890 },
    { 7.985487, 0.210153 },    { 8.193746, 0.195300 },    { 8.427372, 0.200768 },    { 8.600757, 0.193969 },    { 8.833475, 0.150210 },
    { 9.043225, 0.169537 },    { 9.242288, 0.172941 },    { 9.358240, 0.156692 },    { 9.656262, 0.159518 },    { 9.816246, 0.134652 },
    { 9.968577, 0.114214 },
} };
#endif

static size_t sg_nCalls = 0;

// Minimize this
struct BlackBoxCostFunctor 
{

    template <typename T>
    bool operator()(
        const T* const a1,
        const T* const l1,
        const T* const a2,
        const T* const l2,
        T* residual
    ) const
    {
        double cost = 0.0;
        for (const auto& [x, y] : timeline) {
#if 0
             cost += abs(a1[0] * exp(l1[0] * x)
                       + a2[0] * exp(l2[0] * x)
                       - y);
#else
            double err = a1[0] * exp(l1[0] * x)
                       + a2[0] * exp(l2[0] * x)
                       - y;
            cost += err * err;
#endif
        }
        residual[0] = cost;
        ++sg_nCalls;
        return true;
    }

};


int main(int /*argc*/, char** /*argv*/)
{
    double a1 = timeline[0].second;
    double l1 = 0.0;
    double a2 = 0.0;
    double l2 = 0.0;

    // Ceres solver wrapper
    BlackBoxCostFunctor bb;
    auto wrapperCostFn = [&bb](
        const std::vector<double>& v, void*, bool* stopReq
        ) -> double
        {
            double result;
            bb(&v[0], &v[1], &v[2], &v[3], &result);
            return result;
        };

    constexpr double ad = 0.2, ld = 0.01; // deltas
    std::vector<std::vector<double>> simplex = {
        { a1, l1, a2, l2 },
        { ad, .0, .0, .0 },
        { .0, ld, .0, .0 },
        { .0, .0, ad, .0 },
        { .0, .0, .0, ld },
    };
    
    std::cout << "Initial cost = " << wrapperCostFn(simplex[0], nullptr, nullptr) << "\n";

    std::vector<double> coefs(4, 0.0);
    auto timeStart = std::chrono::high_resolution_clock::now();
    std::vector<double> result = BT::Simplex(wrapperCostFn, coefs, 0.000001, simplex, 50000);
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - timeStart).count();

    std::cout << "Duration = " << duration * 1e-9 << "s\n";
    std::cout << "Final cost = " << wrapperCostFn(result, nullptr, nullptr) << "\n";

    //std::cout << summary.BriefReport() << "\n";
    std::cout << "Final solution: \n    "
        << "y = " << result[0] << "*exp(" << result[1] << "*x) + " << result[2] << "*exp(" << result[3] << "*x)\n";

    std::cout << "Number of calls: " << sg_nCalls << "\n";
    return 0;
}
