#ifndef PARAMCONTROLLER_HPP
#define PARAMCONTROLLER_HPP
/*
This file is part of AlgAudio.

AlgAudio, Copyright (C) 2015 CeTA - Audiovisual Technology Center

AlgAudio is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

AlgAudio is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with AlgAudio.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <string>
#include <memory>
#include "ModuleTemplate.hpp"
#include "Utilities.hpp"

namespace AlgAudio{

class Module;
  
/** A ParamController is the representation of a Param state.
 *  All ParamControllers belong to some Module, and are always build according to
 *  a ParramTemplate. The internal state of a ParamController represents it's
 *  current value. A ParamController's value may be Set(...), which sends
 *  the value to SC, or to user-defined custom reaction routine.
 */
class ParamController{
public:
  std::string id;
  static std::shared_ptr<ParamController> Create(std::shared_ptr<Module> m, const std::shared_ptr<ParamTemplate> templ);
  void Set(float value);
  void SetRelative(float value);
  void Reset();
  inline float Get() const {return current_val;}
  float GetRelative() const;
  inline void SetRangeMin(float v) {
    range_min = v;
    on_range_min_set.Happen(v);
    //Set(current_val); // Will re-trigger set events with new relative value
  }
  inline void SetRangeMax(float v) {
    range_max = v;
    on_range_max_set.Happen(v);
    //Set(current_val); // Will re-trigger set events with new relative value
  }
  inline float GetRangeMin() const {return range_min;}
  inline float GetRangeMax() const {return range_max;}

  /** This signal passes two values: the absolute value of this param, and the relative fraction
   *  of the range it operates in. CAREFUL when subscribing to this signal: When a param
   *  changes quickly, subscriber will be called frequently. It is not a good idea
   *  to render any text textures in this handler. Schedule it for next Redraw. */
  Signal<float, float> on_set;
  /** Passing values to other controllers should be done once this controller has it value set. */
  Signal<float, float> after_set;
  
  Signal<float> on_range_min_set;
  Signal<float> on_range_max_set;
  
  const std::shared_ptr<ParamTemplate> templ;
private:
  ParamController(std::shared_ptr<Module> m, const std::shared_ptr<ParamTemplate> t);
  float current_val = 0.0;
  float range_min = 0.0, range_max = 1.0;
  std::weak_ptr<Module> module;
};

/** This class represents a single subscription to received SendReply messages
 *  from SC server. A SendReplyController always belongs to a Module, and is
 *  linked to one of it's Params. When the server sends a SendReply, the OSC
 *  listener will call the Got() message of a corresponding SendReplyController,
 *  which, in turn, sets the linked Param.
 */
class SendReplyController{
public:
  std::string id;
  void Got(float v){ controller->Set(v); }
  static std::shared_ptr<SendReplyController> Create(std::shared_ptr<Module> m, std::string id, std::shared_ptr<ParamController> ctrl);
  ~SendReplyController();
private:
  SendReplyController(std::shared_ptr<Module> m, std::string id, std::shared_ptr<ParamController> ctrl);
  int sendreply_id, module_id;
  std::shared_ptr<ParamController> controller;
  std::weak_ptr<Module> module;
};
  
  
} // namespace AlgAudio

#endif // PARAMCONTROLLER_HPP
