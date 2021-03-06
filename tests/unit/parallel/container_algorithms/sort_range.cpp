//  Copyright (c) 2015 John Biddiscombe
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>

// use smaller array sizes for debug tests
#if defined(HPX_DEBUG)
#define HPX_SORT_TEST_SIZE          50000
#define HPX_SORT_TEST_SIZE_STRINGS  10000
#endif

#include "sort_range_tests.hpp"

////////////////////////////////////////////////////////////////////////////////
void test_sort1()
{
    using namespace hpx::parallel;

    // default comparison operator (std::less)
    test_sort1(seq,     int());
    test_sort1(par,     int());
    test_sort1(par_vec, int());

    // default comparison operator (std::less)
    test_sort1(seq,     double());
    test_sort1(par,     double());
    test_sort1(par_vec, double());

    // default comparison operator (std::less)
    test_sort1(seq,     std::string());
    test_sort1(par,     std::string());
    test_sort1(par_vec, std::string());

    // user supplied comparison operator (std::less)
    test_sort1_comp(seq,     int(), std::less<std::size_t>());
    test_sort1_comp(par,     int(), std::less<std::size_t>());
    test_sort1_comp(par_vec, int(), std::less<std::size_t>());

    // user supplied comparison operator (std::greater)
    test_sort1_comp(seq,     double(), std::greater<double>());
    test_sort1_comp(par,     double(), std::greater<double>());
    test_sort1_comp(par_vec, double(), std::greater<double>());

    // default comparison operator (std::less)
    test_sort1_comp(seq,     std::string(), std::greater<std::string>());
    test_sort1_comp(par,     std::string(), std::greater<std::string>());
    test_sort1_comp(par_vec, std::string(), std::greater<std::string>());

    // Async execution, default comparison operator
    test_sort1_async(seq(task), int());
    test_sort1_async(par(task), char());
    test_sort1_async(seq(task), double());
    test_sort1_async(par(task), float());
    test_sort1_async_string(seq(task), std::string());
    test_sort1_async_string(par(task), std::string());

    // Async execution, user comparison operator
    test_sort1_async(seq(task), int(),    std::less<unsigned int>());
    test_sort1_async(par(task), char(),   std::less<char>());
    //
    test_sort1_async(seq(task), double(), std::greater<double>());
    test_sort1_async(par(task), float(),  std::greater<float>());
    //
    test_sort1_async_string(seq(task), std::string(), std::greater<std::string>());
    test_sort1_async_string(par(task), std::string(), std::greater<std::string>());

    test_sort1(execution_policy(seq),       int());
    test_sort1(execution_policy(par),       int());
    test_sort1(execution_policy(par_vec),   int());
    test_sort1(execution_policy(seq(task)), int());
    test_sort1(execution_policy(par(task)), int());
    test_sort1(execution_policy(seq(task)), std::string());
    test_sort1(execution_policy(par(task)), std::string());
}

void test_sort2()
{
    using namespace hpx::parallel;
    // default comparison operator (std::less)
    test_sort2(seq,     int());
    test_sort2(par,     int());
    test_sort2(par_vec, int());

    // default comparison operator (std::less)
    test_sort2(seq,     double());
    test_sort2(par,     double());
    test_sort2(par_vec, double());

    // user supplied comparison operator (std::less)
    test_sort2_comp(seq,     int(), std::less<std::size_t>());
    test_sort2_comp(par,     int(), std::less<std::size_t>());
    test_sort2_comp(par_vec, int(), std::less<std::size_t>());

    // user supplied comparison operator (std::greater)
    test_sort2_comp(seq,     double(), std::greater<double>());
    test_sort2_comp(par,     double(), std::greater<double>());
    test_sort2_comp(par_vec, double(), std::greater<double>());

    // Async execution, default comparison operator
    test_sort2_async(seq(task), int());
    test_sort2_async(par(task), char());
    test_sort2_async(seq(task), double());
    test_sort2_async(par(task), float());

    // Async execution, user comparison operator
    test_sort2_async(seq(task), int(),    std::less<unsigned int>());
    test_sort2_async(par(task), char(),   std::less<char>());
    //
    test_sort2_async(seq(task), double(), std::greater<double>());
    test_sort2_async(par(task), float(),  std::greater<float>());

    test_sort2(execution_policy(seq),       int());
    test_sort2(execution_policy(par),       int());
    test_sort2(execution_policy(par_vec),   int());
    test_sort2(execution_policy(seq(task)), int());
    test_sort2(execution_policy(par(task)), int());
}

////////////////////////////////////////////////////////////////////////////////
int hpx_main(boost::program_options::variables_map& vm)
{
    unsigned int seed = (unsigned int)std::time(0);
    if (vm.count("seed"))
        seed = vm["seed"].as<unsigned int>();

    std::cout << "using seed: " << seed << std::endl;
    std::srand(seed);

    test_sort1();
    test_sort2();
    return hpx::finalize();
}

int main(int argc, char* argv[])
{
    // add command line option which controls the random number generator seed
    using namespace boost::program_options;
    options_description desc_commandline(
        "Usage: " HPX_APPLICATION_STRING " [options]");

    desc_commandline.add_options()
        ("seed,s", value<unsigned int>(),
        "the random number generator seed to use for this run")
        ;

    // By default this test should run on all available cores
    std::vector<std::string> cfg;
    cfg.push_back("hpx.os_threads=" +
        std::to_string(hpx::threads::hardware_concurrency()));

    HPX_TEST_EQ_MSG(hpx::init(desc_commandline, argc, argv, cfg), 0,
        "HPX main exited with non-zero status");

    return hpx::util::report_errors();
}
