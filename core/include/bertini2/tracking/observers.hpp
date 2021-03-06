//This file is part of Bertini 2.
//
//tracking/observers.hpp is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//tracking/observers.hpp is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with tracking/observers.hpp.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright(C) 2015, 2016 by Bertini2 Development Team
//
// See <http://www.gnu.org/licenses/> for a copy of the license, 
// as well as COPYING.  Bertini2 is provided with permitted 
// additional terms in the b2/licenses/ directory.

// individual authors of this file include:
// daniel brake, university of notre dame


/**
\file tracking/observers.hpp

\brief Contains the tracking/observers base types
*/

#pragma once

#include "bertini2/tracking/events.hpp"
#include "bertini2/tracking/base_tracker.hpp"
#include "bertini2/logging.hpp"
#include <boost/type_index.hpp>

namespace bertini {

	namespace tracking{

		template<class TrackerT>
		class PrecisionAccumulator : public Observer<TrackerT>
		{ BOOST_TYPE_INDEX_REGISTER_CLASS

			using EmitterT = typename TrackerTraits<TrackerT>::EventEmitterType;

			virtual void Observe(AnyEvent const& e) override
			{
				const TrackingEvent<EmitterT>* p = dynamic_cast<const TrackingEvent<EmitterT>*>(&e);
				if (p)
				{
					Visit(p->Get());
				}
			}


			virtual void Visit(TrackerT const& t) override
			{
				precisions_.push_back(t.CurrentPrecision());
			}

		public:
			const std::vector<unsigned>& Precisions() const
			{
				return precisions_;
			}

		private:
			std::vector<unsigned> precisions_;
		};

		/**
		Example usage:
		PathAccumulator<AMPTracker> path_accumulator;
		*/
		template<class TrackerT, template<class> class EventT = SuccessfulStep>
		class AMPPathAccumulator : public Observer<TrackerT>
		{ BOOST_TYPE_INDEX_REGISTER_CLASS

			using EmitterT = typename TrackerTraits<TrackerT>::EventEmitterType;

			virtual void Observe(AnyEvent const& e) override
			{
				const EventT<EmitterT>* p = dynamic_cast<const EventT<EmitterT>*>(&e);
				if (p)
				{
					Visit(p->Get());
				}
			}


			virtual void Visit(TrackerT const& t) override
			{
				path_.push_back(t.CurrentPoint());
			}

		public:
			const std::vector<Vec<mpfr> >& Path() const
			{
				return path_;
			}

		private:
			std::vector<Vec<mpfr> > path_;
		};



		template<class TrackerT>
		class GoryDetailLogger : public Observer<TrackerT>
		{ BOOST_TYPE_INDEX_REGISTER_CLASS
		public:

			using EmitterT = typename TrackerTraits<TrackerT>::EventEmitterType;

			virtual void Observe(AnyEvent const& e) override
			{
				

				if (auto p = dynamic_cast<const Initializing<EmitterT,dbl>*>(&e))
				{
					BOOST_LOG_TRIVIAL(severity_level::debug) << "initializing, tracking path\nfrom\tt = " << p->StartTime() << "\nto\tt = " << p->EndTime() << "\n from\tx = \n" << p->StartPoint();// << "\n\nusing predictor " << p->Get().Predictor();
				}
				else if (auto p = dynamic_cast<const Initializing<EmitterT,mpfr>*>(&e))
				{
					BOOST_LOG_TRIVIAL(severity_level::debug) << "initializing, tracking path\nfrom\tt = " << p->StartTime() << "\nto\tt = " << p->EndTime() << "\n from\tx = \n" << p->StartPoint();// << "\n\nusing predictor " << p->Get().Predictor();
				}

				else if(auto p = dynamic_cast<const TrackingEnded<EmitterT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::trace) << "tracking ended";

				else if (auto p = dynamic_cast<const NewStep<EmitterT>*>(&e))
				{
					auto& t = p->Get();
					BOOST_LOG_TRIVIAL(severity_level::trace) << "Tracker iteration " << t.NumTotalStepsTaken() << "\ncurrent precision: " << t.CurrentPrecision();

					
					BOOST_LOG_TRIVIAL(severity_level::trace) 
						<< "t = " << t.CurrentTime() 
						<< "\ncurrent stepsize: " << t.CurrentStepsize() 
						<< "\ndelta_t = " << t.DeltaT() << "\ncurrent x = "
						<< t.CurrentPoint();
				}



				else if (auto p = dynamic_cast<const SingularStartPoint<EmitterT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::trace) << "singular start point";
				else if (auto p = dynamic_cast<const InfinitePathTruncation<EmitterT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::trace) << "tracker iteration indicated going to infinity, truncated path";




				else if (auto p = dynamic_cast<const SuccessfulStep<EmitterT>*>(&e))
				{
					BOOST_LOG_TRIVIAL(severity_level::trace) << "tracker iteration successful\n\n\n";
				}
				
				else if (auto p = dynamic_cast<const FailedStep<EmitterT>*>(&e))
				{
					BOOST_LOG_TRIVIAL(severity_level::trace) << "tracker iteration unsuccessful\n\n\n";
				}




				

				else if (auto p = dynamic_cast<const SuccessfulPredict<EmitterT,mpfr>*>(&e))
				{
					BOOST_LOG_TRIVIAL(severity_level::trace) << "prediction successful, result:\n" << p->ResultingPoint();
				}
				else if (auto p = dynamic_cast<const SuccessfulPredict<EmitterT,dbl>*>(&e))
				{
					BOOST_LOG_TRIVIAL(severity_level::trace) << "prediction successful, result:\n" << p->ResultingPoint();
				}

				else if (auto p = dynamic_cast<const SuccessfulCorrect<EmitterT,mpfr>*>(&e))
				{
					BOOST_LOG_TRIVIAL(severity_level::trace) << "correction successful, result:\n" << p->ResultingPoint();
				}
				else if (auto p = dynamic_cast<const SuccessfulCorrect<EmitterT,dbl>*>(&e))
				{
					BOOST_LOG_TRIVIAL(severity_level::trace) << "correction successful, result:\n" << p->ResultingPoint();
				}


				else if (auto p = dynamic_cast<const PredictorHigherPrecisionNecessary<EmitterT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::trace) << "Predictor, higher precision necessary";
				else if (auto p = dynamic_cast<const CorrectorHigherPrecisionNecessary<EmitterT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::trace) << "corrector, higher precision necessary";				



				else if (auto p = dynamic_cast<const CorrectorMatrixSolveFailure<EmitterT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::trace) << "corrector, matrix solve failure or failure to converge";				
				else if (auto p = dynamic_cast<const PredictorMatrixSolveFailure<EmitterT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::trace) << "predictor, matrix solve failure or failure to converge";	
				else if (auto p = dynamic_cast<const FirstStepPredictorMatrixSolveFailure<EmitterT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::trace) << "Predictor, matrix solve failure in initial solve of prediction";	

					
				else if (auto p = dynamic_cast<const PrecisionChanged<EmitterT>*>(&e))
					BOOST_LOG_TRIVIAL(severity_level::debug) << "changing precision from " << p->Previous() << " to " << p->Next();
				
				else
					BOOST_LOG_TRIVIAL(severity_level::debug) << "unlogged event, of type: " << boost::typeindex::type_id_runtime(e).pretty_name();
			}

			virtual void Visit(TrackerT const& t) override
			{}
		};



		template<class TrackerT>
		class StepFailScreenPrinter : public Observer<TrackerT>
		{ BOOST_TYPE_INDEX_REGISTER_CLASS
		public:
			
			using EmitterT = typename TrackerTraits<TrackerT>::EventEmitterType;

			virtual void Observe(AnyEvent const& e) override
			{
				if (auto p = dynamic_cast<const FailedStep<EmitterT>*>(&e))
					std::cout << "observed step failure" << std::endl;
			}

			virtual void Visit(TrackerT const& t) override
			{}
		};

	} //re: namespace tracking

}// re: namespace bertini
