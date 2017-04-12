//This file is part of Bertini 2.
//
//bertini2/io/parsing/settings_rules/endgames.hpp is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//bertini2/io/parsing/setting_rules/endgames.hpp is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with bertini2/io/parsing/setting_rules/endgames.hpp.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright(C) 2015 - 2017 by Bertini2 Development Team
//
// See <http://www.gnu.org/licenses/> for a copy of the license,
// as well as COPYING.  Bertini2 is provided with permitted
// additional terms in the b2/licenses/ directory.


/**
 \file bertini2/io/parsing/setting_rules/endgames.hpp
 
 \brief Provides the parsing rules for endgames-related settings in bertini2.
 */

#pragma once


#include "bertini2/io/parsing/settings_rules/base.hpp"
#include "bertini2/endgames/config.hpp"


namespace bertini {
	namespace parsing {
		namespace classic {

			
			
			
			
			
			
			
			
			/**

			 */
			template<typename Iterator, typename T, typename Skipper> //boost::spirit::unused_type
			struct ConfigSettingParser<Iterator, config::Security<T>, T, Skipper> : qi::grammar<Iterator, config::Security<T>(), Skipper>
			{
				
				ConfigSettingParser() : ConfigSettingParser::base_type(root_rule_, "ConfigSecurityType")
				{
					namespace phx = boost::phoenix;
					using qi::_1;
					using qi::_2;
					using qi::_3;
					using qi::_4;
					using qi::_val;
					using qi::eps;
					using qi::lit;
					using qi::char_;
					using qi::omit;
					using boost::spirit::lexeme;
					using boost::spirit::as_string;
					using boost::spirit::ascii::no_case;
					
					
					
					std::string level_name = "securitylevel";
					std::string maxnorm_name = "securitymaxnorm";
					
					
					root_rule_.name("ConfigSecurity_root_rule");
					
					root_rule_ = ((security_level_[phx::bind( [this](config::Security<T> & S, int l)
															 {
																 S.level = l;
															 }, _val, _1 )]
								   ^ security_max_norm_[phx::bind( [this](config::Security<T> & S, T norm)
																  {
																	  S.max_norm = norm;
																  }, _val, _1 )])
								  >> -no_setting_) | no_setting_;
					
					all_names_ = (no_case[level_name] >> ':') | (no_case[maxnorm_name] >> ':');
					
					security_level_.name("security_level_");
					security_level_ = *(char_ - all_names_) >> (no_case[level_name] >> ':') >> qi::int_[_val = _1] >> ';';
					
					security_max_norm_.name("security_max_norm_");
					security_max_norm_ = *(char_ - all_names_) >> (no_case[maxnorm_name] >> ':')
					>> mpfr_rules.number_string_[phx::bind( [this](T & num, std::string str)
														   {
															   num = bertini::NumTraits<T>::FromString(str);
														   }, _val, _1 )] >> ';';
					
					no_setting_.name("no_setting_");
					no_setting_ = *(char_ - all_names_);
					
					no_decl_.name("no_decl_");
					no_decl_ = *(char_);
					
					
					
					
					using phx::val;
					using phx::construct;
					using namespace qi::labels;
					qi::on_error<qi::fail>
					( root_rule_ ,
					 std::cout<<
					 val("config parser could not complete parsing. Expecting ")<<
					 _4<<
					 val(" here: ")<<
					 construct<std::string>(_3,_2)<<
					 std::endl
					 );
					
					
					
				}
				
				
			private:
				qi::rule<Iterator, config::Security<T>(), ascii::space_type > root_rule_;
				qi::rule<Iterator, int(), ascii::space_type > security_level_;
				qi::rule<Iterator, T(), ascii::space_type > security_max_norm_;
				qi::rule<Iterator, ascii::space_type, std::string()> no_decl_, no_setting_, all_names_;
				MPParserRules<Iterator> mpfr_rules;
				
				
				
			}; //re: SecurityParser
			
			
			
			
			
			/**

			 */
			template<typename Iterator, typename T, typename Skipper> //boost::spirit::unused_type
			struct ConfigSettingParser<Iterator, config::Endgame<T>, T, Skipper> : qi::grammar<Iterator, config::Endgame<T>(), Skipper>
			{
				
				ConfigSettingParser() : ConfigSettingParser::base_type(root_rule_, "ConfigEndgameType")
				{
					namespace phx = boost::phoenix;
					using qi::_1;
					using qi::_2;
					using qi::_3;
					using qi::_4;
					using qi::_val;
					using qi::eps;
					using qi::lit;
					using qi::char_;
					using qi::omit;
					using boost::spirit::lexeme;
					using boost::spirit::as_string;
					using boost::spirit::ascii::no_case;
					
					
					
					std::string samplefactor_name = "samplefactor";
					std::string numpoints_name = "numsamplepoints";
					std::string mintrack_name = "nbhdradius";
					
					
					root_rule_.name("ConfigEndgame_root_rule");
					
					root_rule_ = ((sample_factor_[phx::bind( [this](config::Endgame<T> & S, T num)
															{
																S.sample_factor = num;
															}, _val, _1 )]
								   ^ min_track_[phx::bind( [this](config::Endgame<T> & S, T num)
														  {
															  S.min_track_time = num;
														  }, _val, _1 )]
								   ^ num_sample_[phx::bind( [this](config::Endgame<T> & S, unsigned num)
															  {
																  S.num_sample_points = num;
															  }, _val, _1 )])
								  
								  >> -no_setting_)
					| no_setting_;
					
					
					all_names_ = (no_case[samplefactor_name] >> ':') | (no_case[numpoints_name] >> ':')| (no_case[mintrack_name] >> ':');
					
					sample_factor_.name("sample_factor_");
					sample_factor_ = *(char_ - all_names_) >> (no_case[samplefactor_name] >> ':')
					>> mpfr_rules.number_string_[phx::bind( [this](T & num, std::string str)
														   {
															   num = bertini::NumTraits<T>::FromString(str);
														   }, _val, _1 )] >> ';';
					
					min_track_.name("min_track_");
					min_track_ = *(char_ - all_names_) >> (no_case[mintrack_name] >> ':')
					>> mpfr_rules.number_string_[phx::bind( [this](T & num, std::string str)
														   {
															   num = bertini::NumTraits<T>::FromString(str);
														   }, _val, _1 )] >> ';';
					
					num_sample_.name("num_sample_");
					num_sample_ = *(char_ - all_names_) >> (no_case[numpoints_name] >> ':')
					>> qi::uint_[_val=_1] >> ';';
					
					no_setting_.name("no_setting_");
					no_setting_ = *(char_ - all_names_);
					
					no_decl_.name("no_decl_");
					no_decl_ = *(char_);
					
					
					
					
					
					using phx::val;
					using phx::construct;
					using namespace qi::labels;
					qi::on_error<qi::fail>
					( root_rule_ ,
					 std::cout<<
					 val("config parser could not complete parsing. Expecting ")<<
					 _4<<
					 val(" here: ")<<
					 construct<std::string>(_3,_2)<<
					 std::endl
					 );
					
					
					
				}
				
				
			private:
				qi::rule<Iterator, config::Endgame<T>(), ascii::space_type > root_rule_;
				qi::rule<Iterator, T(), ascii::space_type > sample_factor_, min_track_;
				qi::rule<Iterator, unsigned int(), ascii::space_type > num_sample_;
				qi::rule<Iterator, ascii::space_type, std::string()> no_decl_, no_setting_, all_names_;
				MPParserRules<Iterator> mpfr_rules;
			}; //re: EndgameParser
			
			
			
			
			
			/**

			 */
			template<typename Iterator, typename T, typename Skipper> //boost::spirit::unused_type
			struct ConfigSettingParser<Iterator, config::PowerSeries, T, Skipper> : qi::grammar<Iterator, config::PowerSeries(), Skipper>
			{
				
				ConfigSettingParser() : ConfigSettingParser::base_type(root_rule_, "ConfigPowerSeriesType")
				{
					namespace phx = boost::phoenix;
					using qi::_1;
					using qi::_2;
					using qi::_3;
					using qi::_4;
					using qi::_val;
					using qi::eps;
					using qi::lit;
					using qi::char_;
					using qi::omit;
					using boost::spirit::lexeme;
					using boost::spirit::as_string;
					using boost::spirit::ascii::no_case;
					
					
					
					std::string maxcycle_name = "maxcyclenum";
					
					
					root_rule_.name("ConfigPowerSeries_root_rule");
					
					root_rule_ = (max_cycle_[phx::bind( [this](config::PowerSeries & S, unsigned num)
													   {
														   S.max_cycle_number = num;
													   }, _val, _1 )]
								  
								  >> -no_setting_)
					| no_setting_;
					
					
					
					all_names_ = eps >> (no_case[maxcycle_name] >> ':');
					
					max_cycle_.name("max_cycle_");
					max_cycle_ = *(char_ - all_names_) >> (no_case[maxcycle_name] >> ':') >> qi::uint_[_val=_1] >> ';';
					
					no_setting_.name("no_setting_");
					no_setting_ = *(char_ - all_names_);
					
					no_decl_.name("no_decl_");
					no_decl_ = *(char_);
					
					
					
					
					
					using phx::val;
					using phx::construct;
					using namespace qi::labels;
					qi::on_error<qi::fail>
					( root_rule_ ,
					 std::cout<<
					 val("config parser could not complete parsing. Expecting ")<<
					 _4<<
					 val(" here: ")<<
					 construct<std::string>(_3,_2)<<
					 std::endl
					 );
					
					
					
				}
				
				
			private:
				qi::rule<Iterator, config::PowerSeries(), Skipper> root_rule_;
				qi::rule<Iterator, unsigned int(), ascii::space_type > max_cycle_;
				qi::rule<Iterator, ascii::space_type, std::string()> no_decl_, no_setting_, all_names_;
			}; //re: PowerSeriesParser
			
			
			
			
			
			/**

			 */
			template<typename Iterator, typename T, typename Skipper> //boost::spirit::unused_type
			struct ConfigSettingParser<Iterator, config::Cauchy<T>, T, Skipper> : qi::grammar<Iterator, config::Cauchy<T>(), Skipper>
			{
				
				ConfigSettingParser() : ConfigSettingParser::base_type(root_rule_, "ConfigCauchyType")
				{
					namespace phx = boost::phoenix;
					using qi::_1;
					using qi::_2;
					using qi::_3;
					using qi::_4;
					using qi::_val;
					using qi::eps;
					using qi::lit;
					using qi::char_;
					using qi::omit;
					using boost::spirit::lexeme;
					using boost::spirit::as_string;
					using boost::spirit::ascii::no_case;
					
					
					
					std::string cyclecutoff_name = "cycletimecutoff";
					std::string ratiocutoff_name = "ratiotimecutoff";
					
					
					root_rule_.name("ConfigCauchy_root_rule");
					
					root_rule_ = ((cycle_cutoff_[phx::bind( [this](config::Cauchy<T> & S, T num)
														   {
															   S.cycle_cutoff_time = num;
														   }, _val, _1 )]
								   ^ ratio_cutoff_[phx::bind( [this](config::Cauchy<T> & S, T num)
															 {
																 S.ratio_cutoff_time = num;
															 }, _val, _1 )])
								  >> -no_setting_) | no_setting_;
					
					all_names_ = (no_case[cyclecutoff_name] >> ':') | (no_case[ratiocutoff_name] >> ':');
					
					cycle_cutoff_.name("cycle_cutoff_");
					cycle_cutoff_ = *(char_ - all_names_) >> (no_case[cyclecutoff_name] >> ':')
					>> mpfr_rules.number_string_[phx::bind( [this](T & num, std::string str)
														   {
															   num = bertini::NumTraits<T>::FromString(str);
														   }, _val, _1 )] >> ';';
					
					ratio_cutoff_.name("ratio_cutoff_");
					ratio_cutoff_ = *(char_ - all_names_) >> (no_case[ratiocutoff_name] >> ':')
					>> mpfr_rules.number_string_[phx::bind( [this](T & num, std::string str)
														   {
															   num = bertini::NumTraits<T>::FromString(str);
														   }, _val, _1 )] >> ';';
					
					no_setting_.name("no_setting_");
					no_setting_ = *(char_ - all_names_);
					
					no_decl_.name("no_decl_");
					no_decl_ = *(char_);
					
					
					
					
					using phx::val;
					using phx::construct;
					using namespace qi::labels;
					qi::on_error<qi::fail>
					( root_rule_ ,
					 std::cout<<
					 val("config parser could not complete parsing. Expecting ")<<
					 _4<<
					 val(" here: ")<<
					 construct<std::string>(_3,_2)<<
					 std::endl
					 );
					
					
					
				}
				
				
			private:
				qi::rule<Iterator, config::Cauchy<T>(), ascii::space_type > root_rule_;
				qi::rule<Iterator, T(), ascii::space_type > cycle_cutoff_, ratio_cutoff_;
				qi::rule<Iterator, ascii::space_type, std::string()> no_decl_, no_setting_, all_names_;
				MPParserRules<Iterator> mpfr_rules;
				
				
				
			}; //re: CauchyParser

			

			template<typename Iterator, typename T, typename Skipper> //boost::spirit::unused_type
			struct ConfigSettingParser<Iterator, config::TrackBack, T, Skipper> : qi::grammar<Iterator, config::TrackBack(), Skipper>
			{
				
				ConfigSettingParser() : ConfigSettingParser::base_type(root_rule_, "ConfigTrackBack")
				{
					namespace phx = boost::phoenix;
					using qi::_1;
					using qi::_2;
					using qi::_3;
					using qi::_4;
					using qi::_val;
					using qi::eps;
					using qi::lit;
					using qi::char_;
					using qi::omit;
					using boost::spirit::lexeme;
					using boost::spirit::as_string;
					using boost::spirit::ascii::no_case;
					
					
					
					std::string min_cycle_name = "mincycletrackback";
					std::string junk_removal_name = "junkremovaltrackback";
					std::string maxdepth_name = "maxldtdepth";

					root_rule_.name("ConfigTrackBack_root_rule");
					
					root_rule_ = root_rule_ = ((min_cycle_[phx::bind( [this](config::TrackBack & S, unsigned num)
																	   {
																		   S.minimum_cycle = num;
																	   }, _val, _1 )]
											   ^ junk_removal_[phx::bind( [this](config::TrackBack & S, int num)
																		 {
																			 S.junk_removal_test = static_cast<bool>(num);
																		 }, _val, _1 )]
											   ^ max_depth_[phx::bind( [this](config::TrackBack & S, unsigned num)
																	   {
																		   S.max_depth_LDT = num;
																	   }, _val, _1 )]
											  )
											  >> -no_setting_) | no_setting_;
					
					
					
					all_names_ = eps >> (no_case[min_cycle_name] >> ':') |
										(no_case[junk_removal_name] >> ':') |
										(no_case[maxdepth_name] >> ':');
					
					min_cycle_.name("min_cycle_");
					min_cycle_ = *(char_ - all_names_) >> (no_case[min_cycle_name] >> ':') >> qi::uint_[_val=_1] >> ';';
					
					junk_removal_.name("junk_removal_");
					junk_removal_ = *(char_ - all_names_) >> (no_case[junk_removal_name] >> ':') >> qi::int_[_val=_1] >> ';';

					max_depth_.name("max_depth_");
					max_depth_ = *(char_ - all_names_) >> (no_case[maxdepth_name] >> ':') >> qi::uint_[_val=_1] >> ';';

					no_setting_.name("no_setting_");
					no_setting_ = *(char_ - all_names_);
					
					no_decl_.name("no_decl_");
					no_decl_ = *(char_);
					
					
					
					
					
					using phx::val;
					using phx::construct;
					using namespace qi::labels;
					qi::on_error<qi::fail>
					( root_rule_ ,
					 std::cout<<
					 val("config parser could not complete parsing. Expecting ")<<
					 _4<<
					 val(" here: ")<<
					 construct<std::string>(_3,_2)<<
					 std::endl
					 );
					
					
					
				}
				
				
			private:
				qi::rule<Iterator, config::TrackBack(), Skipper> root_rule_;
				qi::rule<Iterator, unsigned int(), ascii::space_type > min_cycle_, max_depth_;
				qi::rule<Iterator, int(), ascii::space_type > junk_removal_;
				qi::rule<Iterator, ascii::space_type, std::string()> no_decl_, no_setting_, all_names_;
			}; //re: TrackBackParser





		} // re: namespace classic
		
	}// re: namespace parsing
}// re: namespace bertini