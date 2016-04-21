//This file is part of Bertini 2.
//
//euler_test.cpp is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//euler_test.cpp is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with euler_test.cpp.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright(C) 2015, 2016 by Bertini2 Development Team
//
// See <http://www.gnu.org/licenses/> for a copy of the license,
// as well as COPYING.  Bertini2 is provided with permitted
// additional terms in the b2/licenses/ directory.

// individual authors of this file include:
// daniel brake, university of notre dame





#include <boost/test/unit_test.hpp>

#include <boost/multiprecision/mpfr.hpp>
#include "limbo.hpp"
#include "mpfr_complex.hpp"

#include "tracking/predict.hpp"


using System = bertini::System;
using Variable = bertini::node::Variable;
using Float = bertini::node::Float;

using Var = std::shared_ptr<Variable>;

using VariableGroup = bertini::VariableGroup;


using dbl = std::complex<double>;
using mpfr = bertini::complex;
using mpfr_float = bertini::mpfr_float;


template<typename NumType> using Vec = bertini::Vec<NumType>;
template<typename NumType> using Mat = bertini::Mat<NumType>;

extern double threshold_clearance_d;
extern bertini::mpfr_float threshold_clearance_mp;
extern unsigned TRACKING_TEST_MPFR_DEFAULT_DIGITS;




BOOST_AUTO_TEST_SUITE(higher_predict_tracking_basics)



/////////////////  RK4  //////////////////////////
BOOST_AUTO_TEST_CASE(circle_line_RK4_double)
{
	
	// Starting point in spacetime step
	Vec<dbl> current_space(2);
	current_space << dbl(2.3,0.2), dbl(1.1, 1.87);
	
	// Starting time
	dbl current_time(0.9);
	// Time step
	dbl delta_t(-0.1);
	
	
	
	
	bertini::System sys;
	Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
	
	VariableGroup vars{x,y};
	
	sys.AddVariableGroup(vars);
	sys.AddPathVariable(t);
	
	// Define homotopy system
	sys.AddFunction( t*(pow(x,2)-1) + (1-t)*(pow(x,2) + pow(y,2) - 4) );
	sys.AddFunction( t*(y-1) + (1-t)*(2*x + 5*y) );
	
	
	auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
	
	BOOST_CHECK_EQUAL(AMP.degree_bound,2);
	AMP.coefficient_bound = 5;
	
	double norm_J, norm_J_inverse, size_proportion;
	
	Vec<dbl> predicted(2);
	predicted << dbl(2.39187197874999601460772208561997,0.215631510575697758920211277830812),
	dbl(0.524028449166667552309395092084449, 1.42874855320540049801773082714871);
	
	Vec<dbl> RK4_prediction_result;
	dbl next_time;
	
	double tracking_tolerance(1e-5);
	double condition_number_estimate;
	unsigned num_steps_since_last_condition_number_computation = 1;
	unsigned frequency_of_CN_estimation = 1;
	
	auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RK4,
												   RK4_prediction_result,
												   size_proportion,
												   norm_J, norm_J_inverse,
												   sys,
												   current_space, current_time,
												   delta_t,
												   condition_number_estimate,
												   num_steps_since_last_condition_number_computation,
												   frequency_of_CN_estimation,
												   tracking_tolerance,
												   AMP);
	
	BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
	BOOST_CHECK_EQUAL(RK4_prediction_result.size(),2);
	for (unsigned ii = 0; ii < RK4_prediction_result.size(); ++ii)
		BOOST_CHECK(abs(RK4_prediction_result(ii)-predicted(ii)) < threshold_clearance_d);
	
	}
	
	
	BOOST_AUTO_TEST_CASE(circle_line_RK4_mp)
	{
		bertini::mpfr_float::default_precision(TRACKING_TEST_MPFR_DEFAULT_DIGITS);
		
		// Starting point in spacetime step
		Vec<mpfr> current_space(2);
		current_space << mpfr("2.3","0.2"), mpfr("1.1", "1.87");
		
		// Starting time
		mpfr current_time("0.9");
		// Time step
		mpfr delta_t("-0.1");
		
		
		
		
		bertini::System sys;
		Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
		
		VariableGroup vars{x,y};
		
		sys.AddVariableGroup(vars);
		sys.AddPathVariable(t);
		
		// Define homotopy system
		sys.AddFunction( t*(pow(x,2)-1) + (1-t)*(pow(x,2) + pow(y,2) - 4) );
		sys.AddFunction( t*(y-1) + (1-t)*(2*x + 5*y) );
		
		
		auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
		
		BOOST_CHECK_EQUAL(AMP.degree_bound,2);
		AMP.coefficient_bound = 5;
		
		mpfr_float norm_J, norm_J_inverse, size_proportion;
		
		Vec<mpfr> predicted(2);
		predicted << mpfr("2.39187197874999601460772208561997","0.215631510575697758920211277830812"),
		mpfr("0.524028449166667552309395092084449", "1.42874855320540049801773082714871");
		
		Vec<mpfr> RK4_prediction_result;
		mpfr next_time;
		
		mpfr_float tracking_tolerance("1e-5");
		mpfr_float condition_number_estimate;
		unsigned num_steps_since_last_condition_number_computation = 1;
		unsigned frequency_of_CN_estimation = 1;
		
		auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RK4,
													   RK4_prediction_result,
													   size_proportion,
													   norm_J, norm_J_inverse,
													   sys,
													   current_space, current_time,
													   delta_t,
													   condition_number_estimate,
													   num_steps_since_last_condition_number_computation,
													   frequency_of_CN_estimation,
													   tracking_tolerance,
													   AMP);
		
		BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
		BOOST_CHECK_EQUAL(RK4_prediction_result.size(),2);
		for (unsigned ii = 0; ii < RK4_prediction_result.size(); ++ii)
			BOOST_CHECK(abs(RK4_prediction_result(ii)-predicted(ii)) < threshold_clearance_mp);
		
	}
	
	
	
	
	
	
	
	BOOST_AUTO_TEST_CASE(monodromy_RK4_d)
	{
		boost::multiprecision::mpfr_float::default_precision(TRACKING_TEST_MPFR_DEFAULT_DIGITS);
		
		// Starting point in spacetime step
		Vec<dbl> current_space(2);
		current_space << dbl(4.641588833612776e-1), dbl(7.416198487095662e-1);
		
		// Starting time
		dbl current_time(0.7);
		// Time step
		dbl delta_t(-0.01);
		
		
		
		
		bertini::System sys;
		Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
		
		VariableGroup vars{x,y};
		
		sys.AddVariableGroup(vars);
		sys.AddPathVariable(t);
		
		// Define homotopy system
		sys.AddFunction( t*(pow(x,3)-1) + (1-t)*(pow(x,3) + 2) );
		sys.AddFunction( t*(pow(y,2)-1) + (1-t)*(pow(y,2) + mpfr_float("0.5")) );
		
		
		auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
		
		BOOST_CHECK_EQUAL(AMP.degree_bound,3);
		AMP.coefficient_bound = 2;
		
		
		Vec<dbl> predicted(2);
		predicted << dbl(0.412127272215145744043367969788438),
		dbl(0.731436941908745436117221142349986);
		
		Vec<dbl> RK4_prediction_result;
		double next_time;
		
		double tracking_tolerance(1e-5);
		double condition_number_estimate;
		unsigned num_steps_since_last_condition_number_computation = 1;
		unsigned frequency_of_CN_estimation = 1;
		
		auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RK4,
													   RK4_prediction_result,
													   sys,
													   current_space, current_time,
													   delta_t,
													   condition_number_estimate,
													   num_steps_since_last_condition_number_computation,
													   frequency_of_CN_estimation,
													   tracking_tolerance,
													   AMP);
		
		BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
		BOOST_CHECK_EQUAL(RK4_prediction_result.size(),2);
		for (unsigned ii = 0; ii < RK4_prediction_result.size(); ++ii)
			BOOST_CHECK(abs(RK4_prediction_result(ii)-predicted(ii)) < threshold_clearance_d);
		
		
	}
	
	
	
	
	BOOST_AUTO_TEST_CASE(monodromy_RK4_mp)
	{
		boost::multiprecision::mpfr_float::default_precision(TRACKING_TEST_MPFR_DEFAULT_DIGITS);
		
		// Starting point in spacetime step
		Vec<mpfr> current_space(2);
		current_space << mpfr("0.464158883361277585510862309093"), mpfr("0.74161984870956629487113974408");
		
		// Starting time
		mpfr current_time("0.7");
		// Time step
		mpfr delta_t("-0.01");
		
		
		
		
		bertini::System sys;
		Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
		
		VariableGroup vars{x,y};
		
		sys.AddVariableGroup(vars);
		sys.AddPathVariable(t);
		
		// Define homotopy system
		sys.AddFunction( t*(pow(x,3)-1) + (1-t)*(pow(x,3) + 2) );
		sys.AddFunction( t*(pow(y,2)-1) + (1-t)*(pow(y,2) + mpfr_float("0.5")) );
		
		
		auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
		
		BOOST_CHECK_EQUAL(AMP.degree_bound,3);
		AMP.coefficient_bound = 2;
		
		
		Vec<mpfr> predicted(2);
		predicted << mpfr("0.412127272215145744043367969788438"),
		mpfr("0.731436941908745436117221142349986");
		
		Vec<mpfr> RK4_prediction_result;
		mpfr next_time;
		
		mpfr_float tracking_tolerance("1e-5");
		mpfr_float condition_number_estimate;
		unsigned num_steps_since_last_condition_number_computation = 1;
		unsigned frequency_of_CN_estimation = 1;
		
		auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RK4,
													   RK4_prediction_result,
													   sys,
													   current_space, current_time,
													   delta_t,
													   condition_number_estimate,
													   num_steps_since_last_condition_number_computation,
													   frequency_of_CN_estimation,
													   tracking_tolerance,
													   AMP);
		
		BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
		BOOST_CHECK_EQUAL(RK4_prediction_result.size(),2);
		for (unsigned ii = 0; ii < RK4_prediction_result.size(); ++ii)
		 BOOST_CHECK(abs(RK4_prediction_result(ii)-predicted(ii)) < threshold_clearance_mp);
	}


	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	

	
	
	///////////////// RKF45 ////////////////////
	
	
	BOOST_AUTO_TEST_CASE(circle_line_RKF45_double)
	{
		
		// Starting point in spacetime step
		Vec<dbl> current_space(2);
		current_space << dbl(2.3,0.2), dbl(1.1, 1.87);
		
		// Starting time
		dbl current_time(0.9);
		// Time step
		dbl delta_t(-0.1);
		
		
		
		
		bertini::System sys;
		Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
		
		VariableGroup vars{x,y};
		
		sys.AddVariableGroup(vars);
		sys.AddPathVariable(t);
		
		// Define homotopy system
		sys.AddFunction( t*(pow(x,2)-1) + (1-t)*(pow(x,2) + pow(y,2) - 4) );
		sys.AddFunction( t*(y-1) + (1-t)*(2*x + 5*y) );
		
		
		auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
		
		BOOST_CHECK_EQUAL(AMP.degree_bound,2);
		AMP.coefficient_bound = 5;
		
		double norm_J, norm_J_inverse, size_proportion, error_est;
		
		Vec<dbl> predicted(2);
		predicted << dbl(2.39189497719010446148169962134860,0.215706089331670902152009632918759),
		dbl(0.524023229576057910435628847490594, 1.42873163348439955724728985152555);
		double predicted_error = 0.0000106466724075688025735071053994891;
		
		Vec<dbl> RKF45_prediction_result;
		dbl next_time;
		
		double tracking_tolerance(1e-5);
		double condition_number_estimate;
		unsigned num_steps_since_last_condition_number_computation = 1;
		unsigned frequency_of_CN_estimation = 1;
		
		auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RKF45,
													   RKF45_prediction_result,
													   error_est,
													   size_proportion,
													   norm_J, norm_J_inverse,
													   sys,
													   current_space, current_time,
													   delta_t,
													   condition_number_estimate,
													   num_steps_since_last_condition_number_computation,
													   frequency_of_CN_estimation,
													   tracking_tolerance,
													   AMP);
		
		BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
		BOOST_CHECK_EQUAL(RKF45_prediction_result.size(),2);
		for (unsigned ii = 0; ii < RKF45_prediction_result.size(); ++ii)
		{
			BOOST_CHECK(abs(RKF45_prediction_result(ii)-predicted(ii)) < threshold_clearance_d);
		}
		BOOST_CHECK(fabs(error_est - predicted_error) < threshold_clearance_d);
		
		
		
	}
	
	
	
	
	
	
	
	
	
	
	BOOST_AUTO_TEST_CASE(circle_line_RKF45_mp)
	{
		bertini::mpfr_float::default_precision(TRACKING_TEST_MPFR_DEFAULT_DIGITS);
		
		// Starting point in spacetime step
		Vec<mpfr> current_space(2);
		current_space << mpfr("2.3","0.2"), mpfr("1.1", "1.87");
		
		// Starting time
		mpfr current_time("0.9");
		// Time step
		mpfr delta_t("-0.1");
		
		
		
		
		bertini::System sys;
		Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
		
		VariableGroup vars{x,y};
		
		sys.AddVariableGroup(vars);
		sys.AddPathVariable(t);
		
		// Define homotopy system
		sys.AddFunction( t*(pow(x,2)-1) + (1-t)*(pow(x,2) + pow(y,2) - 4) );
		sys.AddFunction( t*(y-1) + (1-t)*(2*x + 5*y) );
		
		
		auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
		
		BOOST_CHECK_EQUAL(AMP.degree_bound,2);
		AMP.coefficient_bound = 5;
		
		mpfr_float norm_J, norm_J_inverse, size_proportion, error_est;
		
		Vec<mpfr> predicted(2);
		predicted << mpfr("2.39189497719010446148169962134860","0.215706089331670902152009632918759"),
		mpfr("0.524023229576057910435628847490594", "1.42873163348439955724728985152555");
		mpfr_float predicted_error = mpfr_float("0.0000106466724075688025735071053994891");
		
		Vec<mpfr> RKF45_prediction_result;
		mpfr next_time;
		
		mpfr_float tracking_tolerance("1e-5");
		mpfr_float condition_number_estimate;
		unsigned num_steps_since_last_condition_number_computation = 1;
		unsigned frequency_of_CN_estimation = 1;
		
		auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RKF45,
													   RKF45_prediction_result,
													   error_est,
													   size_proportion,
													   norm_J, norm_J_inverse,
													   sys,
													   current_space, current_time,
													   delta_t,
													   condition_number_estimate,
													   num_steps_since_last_condition_number_computation,
													   frequency_of_CN_estimation,
													   tracking_tolerance,
													   AMP);
		
		BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
		BOOST_CHECK_EQUAL(RKF45_prediction_result.size(),2);
		for (unsigned ii = 0; ii < RKF45_prediction_result.size(); ++ii)
			BOOST_CHECK(abs(RKF45_prediction_result(ii)-predicted(ii)) < threshold_clearance_mp);
		
		BOOST_CHECK(abs(error_est - predicted_error) < threshold_clearance_mp);
		
	}
	
	
	
	
	
	
	
	BOOST_AUTO_TEST_CASE(monodromy_RKF45_d)
	{
		boost::multiprecision::mpfr_float::default_precision(TRACKING_TEST_MPFR_DEFAULT_DIGITS);
		
		// Starting point in spacetime step
		Vec<dbl> current_space(2);
		current_space << dbl(0.464158883361277585510862309093), dbl(0.74161984870956629487113974408);
		
		// Starting time
		dbl current_time(0.7);
		// Time step
		dbl delta_t(-0.01);
		
		
		
		
		bertini::System sys;
		Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
		std::shared_ptr<Float> half = std::make_shared<Float>("0.5");
		
		VariableGroup vars{x,y};
		
		sys.AddVariableGroup(vars);
		sys.AddPathVariable(t);
		
		// Define homotopy system
		sys.AddFunction( t*(pow(x,3)-1) + (1-t)*(pow(x,3) + 2) );
		sys.AddFunction( t*(pow(y,2)-1) + (1-t)*(pow(y,2) + half) );
		
		
		
		auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
		
		double norm_J, norm_J_inverse, size_proportion, error_est;
		
		BOOST_CHECK_EQUAL(AMP.degree_bound,3);
		AMP.coefficient_bound = 2;
		
		
		Vec<dbl> predicted(2);
		predicted << dbl(0.412128542780464095503570026382729),
		dbl(0.731436941916416473300161135742533);
		double predicted_error = 7.17724133646795598396247354053062e-8;
		
		Vec<dbl> RKF45_prediction_result;
		double next_time;
		
		double tracking_tolerance(1e-5);
		double condition_number_estimate;
		unsigned num_steps_since_last_condition_number_computation = 1;
		unsigned frequency_of_CN_estimation = 1;
		
		auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RKF45,
													   RKF45_prediction_result,
													   error_est,
													   size_proportion,
													   norm_J, norm_J_inverse,
													   sys,
													   current_space, current_time,
													   delta_t,
													   condition_number_estimate,
													   num_steps_since_last_condition_number_computation,
													   frequency_of_CN_estimation,
													   tracking_tolerance,
													   AMP);
		
		BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
		BOOST_CHECK_EQUAL(RKF45_prediction_result.size(),2);
		for (unsigned ii = 0; ii < RKF45_prediction_result.size(); ++ii)
		{
			BOOST_CHECK(abs(RKF45_prediction_result(ii)-predicted(ii)) < threshold_clearance_d);
		}
		
		BOOST_CHECK(fabs(error_est - predicted_error) < threshold_clearance_d);
	}
	
	
	
	BOOST_AUTO_TEST_CASE(monodromy_RKF45_mp)
	{
		boost::multiprecision::mpfr_float::default_precision(TRACKING_TEST_MPFR_DEFAULT_DIGITS);
		
		// Starting point in spacetime step
		Vec<mpfr> current_space(2);
		current_space << mpfr("0.464158883361277585510862309093"), mpfr("0.74161984870956629487113974408");
		
		// Starting time
		mpfr current_time("0.7");
		// Time step
		mpfr delta_t("-0.01");
		
		
		
		
		bertini::System sys;
		Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
		std::shared_ptr<Float> half = std::make_shared<Float>("0.5");
		
		VariableGroup vars{x,y};
		
		sys.AddVariableGroup(vars);
		sys.AddPathVariable(t);
		
		// Define homotopy system
		sys.AddFunction( t*(pow(x,3)-1) + (1-t)*(pow(x,3) + 2) );
		sys.AddFunction( t*(pow(y,2)-1) + (1-t)*(pow(y,2) + half) );
		
		
		
		auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
		
		BOOST_CHECK_EQUAL(AMP.degree_bound,3);
		AMP.coefficient_bound = 2;
		
		mpfr_float norm_J, norm_J_inverse, size_proportion, error_est;
		
		
		Vec<mpfr> predicted(2);
		predicted << mpfr("0.412128542780464095503570026382729"),
		mpfr("0.731436941916416473300161135742533");
		mpfr_float predicted_error = mpfr_float("7.17724133646795598396247354053062e-8");
		
		Vec<mpfr> RKF45_prediction_result;
		mpfr next_time;
		
		mpfr_float tracking_tolerance("1e-5");
		mpfr_float condition_number_estimate;
		unsigned num_steps_since_last_condition_number_computation = 1;
		unsigned frequency_of_CN_estimation = 1;
		
		auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RKF45,
													   RKF45_prediction_result,
													   error_est,
													   size_proportion,
													   norm_J, norm_J_inverse,
													   sys,
													   current_space, current_time,
													   delta_t,
													   condition_number_estimate,
													   num_steps_since_last_condition_number_computation,
													   frequency_of_CN_estimation,
													   tracking_tolerance,
													   AMP);
		
		BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
		BOOST_CHECK_EQUAL(RKF45_prediction_result.size(),2);
		for (unsigned ii = 0; ii < RKF45_prediction_result.size(); ++ii)
		{
			BOOST_CHECK(abs(RKF45_prediction_result(ii)-predicted(ii)) < threshold_clearance_mp);
		}
		
		BOOST_CHECK(abs(error_est - predicted_error) < threshold_clearance_mp);
		
	}

























///////////////  RK Cash-Karp 45   ///////////////////
BOOST_AUTO_TEST_CASE(circle_line_RKCK45_double)
{
	
	// Starting point in spacetime step
	Vec<dbl> current_space(2);
	current_space << dbl(2.3,0.2), dbl(1.1, 1.87);
	
	// Starting time
	dbl current_time(0.9);
	// Time step
	dbl delta_t(-0.1);
	
	
	
	
	bertini::System sys;
	Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
	
	VariableGroup vars{x,y};
	
	sys.AddVariableGroup(vars);
	sys.AddPathVariable(t);
	
	// Define homotopy system
	sys.AddFunction( t*(pow(x,2)-1) + (1-t)*(pow(x,2) + pow(y,2) - 4) );
	sys.AddFunction( t*(y-1) + (1-t)*(2*x + 5*y) );
	
	
	auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
	
	BOOST_CHECK_EQUAL(AMP.degree_bound,2);
	AMP.coefficient_bound = 5;
	
	double norm_J, norm_J_inverse, size_proportion, error_est;
	
	Vec<dbl> predicted(2);
	predicted << dbl(2.39189687053703334440737233377404,0.215710089694839238261207432302796),
	dbl(0.524023000601737797891275060536396, 1.42873127596071439996076584113815);
	double predicted_error = 0.00000353010590253211978478006394088836;
	
	Vec<dbl> RKCK45_prediction_result;
	dbl next_time;
	
	double tracking_tolerance(1e-5);
	double condition_number_estimate;
	unsigned num_steps_since_last_condition_number_computation = 1;
	unsigned frequency_of_CN_estimation = 1;
	
	auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RKCashKarp45,
												   RKCK45_prediction_result,
												   error_est,
												   size_proportion,
												   norm_J, norm_J_inverse,
												   sys,
												   current_space, current_time,
												   delta_t,
												   condition_number_estimate,
												   num_steps_since_last_condition_number_computation,
												   frequency_of_CN_estimation,
												   tracking_tolerance,
												   AMP);
	
	BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
	BOOST_CHECK_EQUAL(RKCK45_prediction_result.size(),2);
	for (unsigned ii = 0; ii < RKCK45_prediction_result.size(); ++ii)
	{
		BOOST_CHECK(abs(RKCK45_prediction_result(ii)-predicted(ii)) < threshold_clearance_d);
	}
	BOOST_CHECK(fabs(error_est - predicted_error) < threshold_clearance_d);
	
	
	
}










BOOST_AUTO_TEST_CASE(circle_line_RKCK45_mp)
{
	bertini::mpfr_float::default_precision(TRACKING_TEST_MPFR_DEFAULT_DIGITS);
	
	// Starting point in spacetime step
	Vec<mpfr> current_space(2);
	current_space << mpfr("2.3","0.2"), mpfr("1.1", "1.87");
	
	// Starting time
	mpfr current_time("0.9");
	// Time step
	mpfr delta_t("-0.1");
	
	
	
	
	bertini::System sys;
	Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
	
	VariableGroup vars{x,y};
	
	sys.AddVariableGroup(vars);
	sys.AddPathVariable(t);
	
	// Define homotopy system
	sys.AddFunction( t*(pow(x,2)-1) + (1-t)*(pow(x,2) + pow(y,2) - 4) );
	sys.AddFunction( t*(y-1) + (1-t)*(2*x + 5*y) );
	
	
	auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
	
	BOOST_CHECK_EQUAL(AMP.degree_bound,2);
	AMP.coefficient_bound = 5;
	
	mpfr_float norm_J, norm_J_inverse, size_proportion, error_est;
	
	Vec<mpfr> predicted(2);
	predicted << mpfr("2.39189687053703334440737233377404","0.215710089694839238261207432302796"),
	mpfr("0.524023000601737797891275060536396", "1.42873127596071439996076584113815");
	mpfr_float predicted_error = mpfr_float("0.00000353010590253211978478006394088836");
	
	Vec<mpfr> RKCK45_prediction_result;
	mpfr next_time;
	
	mpfr_float tracking_tolerance("1e-5");
	mpfr_float condition_number_estimate;
	unsigned num_steps_since_last_condition_number_computation = 1;
	unsigned frequency_of_CN_estimation = 1;
	
	auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RKCashKarp45,
												   RKCK45_prediction_result,
												   error_est,
												   size_proportion,
												   norm_J, norm_J_inverse,
												   sys,
												   current_space, current_time,
												   delta_t,
												   condition_number_estimate,
												   num_steps_since_last_condition_number_computation,
												   frequency_of_CN_estimation,
												   tracking_tolerance,
												   AMP);
	
	BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
	BOOST_CHECK_EQUAL(RKCK45_prediction_result.size(),2);
	for (unsigned ii = 0; ii < RKCK45_prediction_result.size(); ++ii)
		BOOST_CHECK(abs(RKCK45_prediction_result(ii)-predicted(ii)) < threshold_clearance_mp);
	
	BOOST_CHECK(abs(error_est - predicted_error) < threshold_clearance_mp);
	
}







BOOST_AUTO_TEST_CASE(monodromy_RKCK45_d)
{
	boost::multiprecision::mpfr_float::default_precision(TRACKING_TEST_MPFR_DEFAULT_DIGITS);
	
	// Starting point in spacetime step
	Vec<dbl> current_space(2);
	current_space << dbl(0.464158883361277585510862309093), dbl(0.74161984870956629487113974408);
	
	// Starting time
	dbl current_time(0.7);
	// Time step
	dbl delta_t(-0.01);
	
	
	
	
	bertini::System sys;
	Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
	std::shared_ptr<Float> half = std::make_shared<Float>("0.5");
	
	VariableGroup vars{x,y};
	
	sys.AddVariableGroup(vars);
	sys.AddPathVariable(t);
	
	// Define homotopy system
	sys.AddFunction( t*(pow(x,3)-1) + (1-t)*(pow(x,3) + 2) );
	sys.AddFunction( t*(pow(y,2)-1) + (1-t)*(pow(y,2) + half) );
	
	
	
	auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
	
	double norm_J, norm_J_inverse, size_proportion, error_est;
	
	BOOST_CHECK_EQUAL(AMP.degree_bound,3);
	AMP.coefficient_bound = 2;
	
	
	Vec<dbl> predicted(2);
	predicted << dbl(0.412128535278042242819741034030722),
	dbl(0.731436941916396784391576913351911);
	double predicted_error = 4.51352044466211707817977052519894e-9;
	
	Vec<dbl> RKCK45_prediction_result;
	double next_time;
	
	double tracking_tolerance(1e-5);
	double condition_number_estimate;
	unsigned num_steps_since_last_condition_number_computation = 1;
	unsigned frequency_of_CN_estimation = 1;
	
	auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RKCashKarp45,
												   RKCK45_prediction_result,
												   error_est,
												   size_proportion,
												   norm_J, norm_J_inverse,
												   sys,
												   current_space, current_time,
												   delta_t,
												   condition_number_estimate,
												   num_steps_since_last_condition_number_computation,
												   frequency_of_CN_estimation,
												   tracking_tolerance,
												   AMP);
	
	BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
	BOOST_CHECK_EQUAL(RKCK45_prediction_result.size(),2);
	for (unsigned ii = 0; ii < RKCK45_prediction_result.size(); ++ii)
	{
		BOOST_CHECK(abs(RKCK45_prediction_result(ii)-predicted(ii)) < threshold_clearance_d);
	}
	
	BOOST_CHECK(fabs(error_est - predicted_error) < threshold_clearance_d);
}



BOOST_AUTO_TEST_CASE(monodromy_RKCK45_mp)
{
	boost::multiprecision::mpfr_float::default_precision(TRACKING_TEST_MPFR_DEFAULT_DIGITS);
	
	// Starting point in spacetime step
	Vec<mpfr> current_space(2);
	current_space << mpfr("0.464158883361277585510862309093"), mpfr("0.74161984870956629487113974408");
	
	// Starting time
	mpfr current_time("0.7");
	// Time step
	mpfr delta_t("-0.01");
	
	
	
	
	bertini::System sys;
	Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
	std::shared_ptr<Float> half = std::make_shared<Float>("0.5");
	
	VariableGroup vars{x,y};
	
	sys.AddVariableGroup(vars);
	sys.AddPathVariable(t);
	
	// Define homotopy system
	sys.AddFunction( t*(pow(x,3)-1) + (1-t)*(pow(x,3) + 2) );
	sys.AddFunction( t*(pow(y,2)-1) + (1-t)*(pow(y,2) + half) );
	
	
	
	auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
	
	BOOST_CHECK_EQUAL(AMP.degree_bound,3);
	AMP.coefficient_bound = 2;
	
	mpfr_float norm_J, norm_J_inverse, size_proportion, error_est;
	
	
	Vec<mpfr> predicted(2);
	predicted << mpfr("0.412128535278042242819741034030722"),
	mpfr("0.731436941916396784391576913351911");
	mpfr_float predicted_error = mpfr_float("4.51352044466211707817977052519894e-9");
	
	Vec<mpfr> RKCK45_prediction_result;
	mpfr next_time;
	
	mpfr_float tracking_tolerance("1e-5");
	mpfr_float condition_number_estimate;
	unsigned num_steps_since_last_condition_number_computation = 1;
	unsigned frequency_of_CN_estimation = 1;
	
	auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RKCashKarp45,
												   RKCK45_prediction_result,
												   error_est,
												   size_proportion,
												   norm_J, norm_J_inverse,
												   sys,
												   current_space, current_time,
												   delta_t,
												   condition_number_estimate,
												   num_steps_since_last_condition_number_computation,
												   frequency_of_CN_estimation,
												   tracking_tolerance,
												   AMP);
	
	BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
	BOOST_CHECK_EQUAL(RKCK45_prediction_result.size(),2);
	for (unsigned ii = 0; ii < RKCK45_prediction_result.size(); ++ii)
	{
		BOOST_CHECK(abs(RKCK45_prediction_result(ii)-predicted(ii)) < threshold_clearance_mp);
	}
	
	BOOST_CHECK(abs(error_est - predicted_error) < threshold_clearance_mp);
	
}

















////////////  RK Dormand Prince 56  /////////////////////

BOOST_AUTO_TEST_CASE(circle_line_RKDP56_double)
{
	
	// Starting point in spacetime step
	Vec<dbl> current_space(2);
	current_space << dbl(2.3,0.2), dbl(1.1, 1.87);
	
	// Starting time
	dbl current_time(0.9);
	// Time step
	dbl delta_t(-0.1);
	
	
	
	
	bertini::System sys;
	Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
	
	VariableGroup vars{x,y};
	
	sys.AddVariableGroup(vars);
	sys.AddPathVariable(t);
	
	// Define homotopy system
	sys.AddFunction( t*(pow(x,2)-1) + (1-t)*(pow(x,2) + pow(y,2) - 4) );
	sys.AddFunction( t*(y-1) + (1-t)*(2*x + 5*y) );
	
	
	auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
	
	BOOST_CHECK_EQUAL(AMP.degree_bound,2);
	AMP.coefficient_bound = 5;
	
	double norm_J, norm_J_inverse, size_proportion, error_est;
	
	Vec<dbl> predicted(2);
	predicted << dbl(2.39189763095027864748166494355925,0.215711752936893239277981497324557),
	dbl(0.524022748677715856115185568097945, 1.42873072156957928016044855615010);
	double predicted_error = 6.79397491522542193110307157970405e-7;
	
	Vec<dbl> RKDP56_prediction_result;
	dbl next_time;
	
	double tracking_tolerance(1e-5);
	double condition_number_estimate;
	unsigned num_steps_since_last_condition_number_computation = 1;
	unsigned frequency_of_CN_estimation = 1;
	
	auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RKDormandPrince56,
												   RKDP56_prediction_result,
												   error_est,
												   size_proportion,
												   norm_J, norm_J_inverse,
												   sys,
												   current_space, current_time,
												   delta_t,
												   condition_number_estimate,
												   num_steps_since_last_condition_number_computation,
												   frequency_of_CN_estimation,
												   tracking_tolerance,
												   AMP);
	
	BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
	BOOST_CHECK_EQUAL(RKDP56_prediction_result.size(),2);
	for (unsigned ii = 0; ii < RKDP56_prediction_result.size(); ++ii)
	{
		BOOST_CHECK(abs(RKDP56_prediction_result(ii)-predicted(ii)) < threshold_clearance_d);
	}
	BOOST_CHECK(fabs(error_est - predicted_error) < threshold_clearance_d);
	
	
	
}










BOOST_AUTO_TEST_CASE(circle_line_RKDP56_mp)
{
	bertini::mpfr_float::default_precision(TRACKING_TEST_MPFR_DEFAULT_DIGITS);
	
	// Starting point in spacetime step
	Vec<mpfr> current_space(2);
	current_space << mpfr("2.3","0.2"), mpfr("1.1", "1.87");
	
	// Starting time
	mpfr current_time("0.9");
	// Time step
	mpfr delta_t("-0.1");
	
	
	
	
	bertini::System sys;
	Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
	
	VariableGroup vars{x,y};
	
	sys.AddVariableGroup(vars);
	sys.AddPathVariable(t);
	
	// Define homotopy system
	sys.AddFunction( t*(pow(x,2)-1) + (1-t)*(pow(x,2) + pow(y,2) - 4) );
	sys.AddFunction( t*(y-1) + (1-t)*(2*x + 5*y) );
	
	
	auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
	
	BOOST_CHECK_EQUAL(AMP.degree_bound,2);
	AMP.coefficient_bound = 5;
	
	mpfr_float norm_J, norm_J_inverse, size_proportion, error_est;
	
	Vec<mpfr> predicted(2);
	predicted << mpfr("2.39189763095027864748166494355925","0.215711752936893239277981497324557"),
	mpfr("0.524022748677715856115185568097945", "1.42873072156957928016044855615010");
	mpfr_float predicted_error = mpfr_float("6.79397491522542193110307157970405e-7");
	
	Vec<mpfr> RKDP56_prediction_result;
	mpfr next_time;
	
	mpfr_float tracking_tolerance("1e-5");
	mpfr_float condition_number_estimate;
	unsigned num_steps_since_last_condition_number_computation = 1;
	unsigned frequency_of_CN_estimation = 1;
	
	auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RKDormandPrince56,
												   RKDP56_prediction_result,
												   error_est,
												   size_proportion,
												   norm_J, norm_J_inverse,
												   sys,
												   current_space, current_time,
												   delta_t,
												   condition_number_estimate,
												   num_steps_since_last_condition_number_computation,
												   frequency_of_CN_estimation,
												   tracking_tolerance,
												   AMP);
	
	BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
	BOOST_CHECK_EQUAL(RKDP56_prediction_result.size(),2);
	for (unsigned ii = 0; ii < RKDP56_prediction_result.size(); ++ii)
		BOOST_CHECK(abs(RKDP56_prediction_result(ii)-predicted(ii)) < threshold_clearance_mp);
	
	BOOST_CHECK(abs(error_est - predicted_error) < threshold_clearance_mp);
	
}







BOOST_AUTO_TEST_CASE(monodromy_RKDP56_d)
{
	boost::multiprecision::mpfr_float::default_precision(TRACKING_TEST_MPFR_DEFAULT_DIGITS);
	
	// Starting point in spacetime step
	Vec<dbl> current_space(2);
	current_space << dbl(0.464158883361277585510862309093), dbl(0.74161984870956629487113974408);
	
	// Starting time
	dbl current_time(0.7);
	// Time step
	dbl delta_t(-0.01);
	
	
	
	
	bertini::System sys;
	Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
	std::shared_ptr<Float> half = std::make_shared<Float>("0.5");
	
	VariableGroup vars{x,y};
	
	sys.AddVariableGroup(vars);
	sys.AddPathVariable(t);
	
	// Define homotopy system
	sys.AddFunction( t*(pow(x,3)-1) + (1-t)*(pow(x,3) + 2) );
	sys.AddFunction( t*(pow(y,2)-1) + (1-t)*(pow(y,2) + half) );
	
	
	
	auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
	
	double norm_J, norm_J_inverse, size_proportion, error_est;
	
	BOOST_CHECK_EQUAL(AMP.degree_bound,3);
	AMP.coefficient_bound = 2;
	
	
	Vec<dbl> predicted(2);
	predicted << dbl(0.412128532164122346459968880922735),
	dbl(0.731436941916392989685864031055020);
	double predicted_error = 3.85904197101299548102733617445410e-9;
	
	Vec<dbl> RKDP56_prediction_result;
	double next_time;
	
	double tracking_tolerance(1e-5);
	double condition_number_estimate;
	unsigned num_steps_since_last_condition_number_computation = 1;
	unsigned frequency_of_CN_estimation = 1;
	
	auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RKDormandPrince56,
												   RKDP56_prediction_result,
												   error_est,
												   size_proportion,
												   norm_J, norm_J_inverse,
												   sys,
												   current_space, current_time,
												   delta_t,
												   condition_number_estimate,
												   num_steps_since_last_condition_number_computation,
												   frequency_of_CN_estimation,
												   tracking_tolerance,
												   AMP);
	
	BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
	BOOST_CHECK_EQUAL(RKDP56_prediction_result.size(),2);
	for (unsigned ii = 0; ii < RKDP56_prediction_result.size(); ++ii)
	{
		BOOST_CHECK(abs(RKDP56_prediction_result(ii)-predicted(ii)) < threshold_clearance_d);
	}
	
	BOOST_CHECK(fabs(error_est - predicted_error) < threshold_clearance_d);
}



BOOST_AUTO_TEST_CASE(monodromy_RKDP56_mp)
{
	boost::multiprecision::mpfr_float::default_precision(TRACKING_TEST_MPFR_DEFAULT_DIGITS);
	
	// Starting point in spacetime step
	Vec<mpfr> current_space(2);
	current_space << mpfr("0.464158883361277585510862309093"), mpfr("0.74161984870956629487113974408");
	
	// Starting time
	mpfr current_time("0.7");
	// Time step
	mpfr delta_t("-0.01");
	
	
	
	
	bertini::System sys;
	Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
	std::shared_ptr<Float> half = std::make_shared<Float>("0.5");
	
	VariableGroup vars{x,y};
	
	sys.AddVariableGroup(vars);
	sys.AddPathVariable(t);
	
	// Define homotopy system
	sys.AddFunction( t*(pow(x,3)-1) + (1-t)*(pow(x,3) + 2) );
	sys.AddFunction( t*(pow(y,2)-1) + (1-t)*(pow(y,2) + half) );
	
	
	
	auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
	
	BOOST_CHECK_EQUAL(AMP.degree_bound,3);
	AMP.coefficient_bound = 2;
	
	mpfr_float norm_J, norm_J_inverse, size_proportion, error_est;
	
	
	Vec<mpfr> predicted(2);
	predicted << mpfr("0.412128532164122346459968880922735"),
	mpfr("0.731436941916392989685864031055020");
	mpfr_float predicted_error = mpfr_float("3.85904197101299548102733617445410e-9");
	
	Vec<mpfr> RKDP56_prediction_result;
	mpfr next_time;
	
	mpfr_float tracking_tolerance("1e-5");
	mpfr_float condition_number_estimate;
	unsigned num_steps_since_last_condition_number_computation = 1;
	unsigned frequency_of_CN_estimation = 1;
	
	auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RKDormandPrince56,
												   RKDP56_prediction_result,
												   error_est,
												   size_proportion,
												   norm_J, norm_J_inverse,
												   sys,
												   current_space, current_time,
												   delta_t,
												   condition_number_estimate,
												   num_steps_since_last_condition_number_computation,
												   frequency_of_CN_estimation,
												   tracking_tolerance,
												   AMP);
	
	BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
	BOOST_CHECK_EQUAL(RKDP56_prediction_result.size(),2);
	for (unsigned ii = 0; ii < RKDP56_prediction_result.size(); ++ii)
	{
		BOOST_CHECK(abs(RKDP56_prediction_result(ii)-predicted(ii)) < threshold_clearance_mp);
	}
	
	BOOST_CHECK(abs(error_est - predicted_error) < threshold_clearance_mp);
	
}












	/////////////////////////////////////////////
	//       RK Verner 67 Tests
	//
	//
	////////////////////////////
BOOST_AUTO_TEST_CASE(circle_line_RKV67_double)
{
	
	// Starting point in spacetime step
	Vec<dbl> current_space(2);
	current_space << dbl(2.3,0.2), dbl(1.1, 1.87);
	
	// Starting time
	dbl current_time(0.9);
	// Time step
	dbl delta_t(-0.1);
	
	
	
	
	bertini::System sys;
	Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
	
	VariableGroup vars{x,y};
	
	sys.AddVariableGroup(vars);
	sys.AddPathVariable(t);
	
	// Define homotopy system
	sys.AddFunction( t*(pow(x,2)-1) + (1-t)*(pow(x,2) + pow(y,2) - 4) );
	sys.AddFunction( t*(y-1) + (1-t)*(2*x + 5*y) );
	
	
	auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
	
	BOOST_CHECK_EQUAL(AMP.degree_bound,2);
	AMP.coefficient_bound = 5;
	
	double norm_J, norm_J_inverse, size_proportion, error_est;
	
	Vec<dbl> predicted(2);
	predicted << dbl(2.39189815934576660586899846426669,0.215712712024488132602524062094065),
	dbl(0.524022631256496309806889230162953, 1.42873050843900263719943909731242);
	double predicted_error = 0.00000128891520195955347062706145253149;
	
	Vec<dbl> RKV67_prediction_result;
	dbl next_time;
	
	double tracking_tolerance(1e-5);
	double condition_number_estimate;
	unsigned num_steps_since_last_condition_number_computation = 1;
	unsigned frequency_of_CN_estimation = 1;
	
	auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RKVerner67,
												   RKV67_prediction_result,
												   error_est,
												   size_proportion,
												   norm_J, norm_J_inverse,
												   sys,
												   current_space, current_time,
												   delta_t,
												   condition_number_estimate,
												   num_steps_since_last_condition_number_computation,
												   frequency_of_CN_estimation,
												   tracking_tolerance,
												   AMP);
	
	BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
	BOOST_CHECK_EQUAL(RKV67_prediction_result.size(),2);
	for (unsigned ii = 0; ii < RKV67_prediction_result.size(); ++ii)
	{
		BOOST_CHECK(abs(RKV67_prediction_result(ii)-predicted(ii)) < threshold_clearance_d);
	}
	BOOST_CHECK(fabs(error_est - predicted_error) < threshold_clearance_d);
	
	
	
	
}










BOOST_AUTO_TEST_CASE(circle_line_RKV67_mp)
{
	bertini::mpfr_float::default_precision(TRACKING_TEST_MPFR_DEFAULT_DIGITS);
	
	// Starting point in spacetime step
	Vec<mpfr> current_space(2);
	current_space << mpfr("2.3","0.2"), mpfr("1.1", "1.87");
	
	// Starting time
	mpfr current_time("0.9");
	// Time step
	mpfr delta_t("-0.1");
	
	
	
	
	bertini::System sys;
	Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
	
	VariableGroup vars{x,y};
	
	sys.AddVariableGroup(vars);
	sys.AddPathVariable(t);
	
	// Define homotopy system
	sys.AddFunction( t*(pow(x,2)-1) + (1-t)*(pow(x,2) + pow(y,2) - 4) );
	sys.AddFunction( t*(y-1) + (1-t)*(2*x + 5*y) );
	
	
	auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
	
	BOOST_CHECK_EQUAL(AMP.degree_bound,2);
	AMP.coefficient_bound = 5;
	
	mpfr_float norm_J, norm_J_inverse, size_proportion, error_est;
	
	Vec<mpfr> predicted(2);
	predicted << mpfr("2.39189815934576660586899846426669","0.215712712024488132602524062094065"),
	mpfr("0.524022631256496309806889230162953", "1.42873050843900263719943909731242");
	mpfr_float predicted_error = mpfr_float("0.00000128891520195955347062706145253149");
	
	Vec<mpfr> RKV67_prediction_result;
	mpfr next_time;
	
	mpfr_float tracking_tolerance("1e-5");
	mpfr_float condition_number_estimate;
	unsigned num_steps_since_last_condition_number_computation = 1;
	unsigned frequency_of_CN_estimation = 1;
	
	auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RKVerner67,
												   RKV67_prediction_result,
												   error_est,
												   size_proportion,
												   norm_J, norm_J_inverse,
												   sys,
												   current_space, current_time,
												   delta_t,
												   condition_number_estimate,
												   num_steps_since_last_condition_number_computation,
												   frequency_of_CN_estimation,
												   tracking_tolerance,
												   AMP);
	
	BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
	BOOST_CHECK_EQUAL(RKV67_prediction_result.size(),2);
	for (unsigned ii = 0; ii < RKV67_prediction_result.size(); ++ii)
		BOOST_CHECK(abs(RKV67_prediction_result(ii)-predicted(ii)) < threshold_clearance_mp);
	
	BOOST_CHECK(abs(error_est - predicted_error) < threshold_clearance_mp);
}







BOOST_AUTO_TEST_CASE(monodromy_RKV67_d)
{
	boost::multiprecision::mpfr_float::default_precision(TRACKING_TEST_MPFR_DEFAULT_DIGITS);
	
	// Starting point in spacetime step
	Vec<dbl> current_space(2);
	current_space << dbl(0.464158883361277585510862309093), dbl(0.74161984870956629487113974408);
	
	// Starting time
	dbl current_time(0.7);
	// Time step
	dbl delta_t(-0.01);
	
	
	
	
	bertini::System sys;
	Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
	std::shared_ptr<Float> half = std::make_shared<Float>("0.5");
	
	VariableGroup vars{x,y};
	
	sys.AddVariableGroup(vars);
	sys.AddPathVariable(t);
	
	// Define homotopy system
	sys.AddFunction( t*(pow(x,3)-1) + (1-t)*(pow(x,3) + 2) );
	sys.AddFunction( t*(pow(y,2)-1) + (1-t)*(pow(y,2) + half) );
	
	
	
	auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
	
	double norm_J, norm_J_inverse, size_proportion, error_est;
	
	BOOST_CHECK_EQUAL(AMP.degree_bound,3);
	AMP.coefficient_bound = 2;
	
	
	Vec<dbl> predicted(2);
	predicted << dbl(0.412128533889452110491490000899263),
	dbl(0.731436941916389669876029584806957);
	double predicted_error = 1.42794733055750714441060080061e-8;
	
	Vec<dbl> RKV67_prediction_result;
	double next_time;
	
	double tracking_tolerance(1e-5);
	double condition_number_estimate;
	unsigned num_steps_since_last_condition_number_computation = 1;
	unsigned frequency_of_CN_estimation = 1;
	
	auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RKVerner67,
												   RKV67_prediction_result,
												   error_est,
												   size_proportion,
												   norm_J, norm_J_inverse,
												   sys,
												   current_space, current_time,
												   delta_t,
												   condition_number_estimate,
												   num_steps_since_last_condition_number_computation,
												   frequency_of_CN_estimation,
												   tracking_tolerance,
												   AMP);
	
	BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
	BOOST_CHECK_EQUAL(RKV67_prediction_result.size(),2);
	for (unsigned ii = 0; ii < RKV67_prediction_result.size(); ++ii)
	{
		BOOST_CHECK(abs(RKV67_prediction_result(ii)-predicted(ii)) < threshold_clearance_d);
	}
	
	BOOST_CHECK(fabs(error_est - predicted_error) < threshold_clearance_d);
}



BOOST_AUTO_TEST_CASE(monodromy_RKV67_mp)
{
	boost::multiprecision::mpfr_float::default_precision(TRACKING_TEST_MPFR_DEFAULT_DIGITS);
	
	// Starting point in spacetime step
	Vec<mpfr> current_space(2);
	current_space << mpfr("0.464158883361277585510862309093"), mpfr("0.74161984870956629487113974408");
	
	// Starting time
	mpfr current_time("0.7");
	// Time step
	mpfr delta_t("-0.01");
	
	
	
	
	bertini::System sys;
	Var x = std::make_shared<Variable>("x"), y = std::make_shared<Variable>("y"), t = std::make_shared<Variable>("t");
	std::shared_ptr<Float> half = std::make_shared<Float>("0.5");
	
	VariableGroup vars{x,y};
	
	sys.AddVariableGroup(vars);
	sys.AddPathVariable(t);
	
	// Define homotopy system
	sys.AddFunction( t*(pow(x,3)-1) + (1-t)*(pow(x,3) + 2) );
	sys.AddFunction( t*(pow(y,2)-1) + (1-t)*(pow(y,2) + half) );
	
	
	
	auto AMP = bertini::tracking::config::AMPConfigFrom(sys);
	
	BOOST_CHECK_EQUAL(AMP.degree_bound,3);
	AMP.coefficient_bound = 2;
	
	mpfr_float norm_J, norm_J_inverse, size_proportion, error_est;
	
	
	Vec<mpfr> predicted(2);
	predicted << mpfr("0.412128533889452110491490000899263"),
	mpfr("0.731436941916389669876029584806957");
	mpfr_float predicted_error = mpfr_float("1.42794733055750714441060080061e-8");
	
	Vec<mpfr> RKV67_prediction_result;
	mpfr next_time;
	
	mpfr_float tracking_tolerance("1e-5");
	mpfr_float condition_number_estimate;
	unsigned num_steps_since_last_condition_number_computation = 1;
	unsigned frequency_of_CN_estimation = 1;
	
	auto success_code = bertini::tracking::Predict(bertini::tracking::config::Predictor::RKVerner67,
												   RKV67_prediction_result,
												   error_est,
												   size_proportion,
												   norm_J, norm_J_inverse,
												   sys,
												   current_space, current_time,
												   delta_t,
												   condition_number_estimate,
												   num_steps_since_last_condition_number_computation,
												   frequency_of_CN_estimation,
												   tracking_tolerance,
												   AMP);
	
	BOOST_CHECK(success_code==bertini::tracking::SuccessCode::Success);
	BOOST_CHECK_EQUAL(RKV67_prediction_result.size(),2);
	for (unsigned ii = 0; ii < RKV67_prediction_result.size(); ++ii)
	{
		BOOST_CHECK(abs(RKV67_prediction_result(ii)-predicted(ii)) < threshold_clearance_mp);
	}
	
	BOOST_CHECK(abs(error_est - predicted_error) < threshold_clearance_mp);
	std::cout << error_est << std::endl;
}


BOOST_AUTO_TEST_SUITE_END()















