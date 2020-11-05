/*

This file is part of VROOM.

Copyright (c) 2015-2020, Julien Coupey.
All rights reserved (see LICENSE).

*/

#include "structures/vroom/vehicle.h"
#include "utils/exception.h"

namespace vroom {

Vehicle::Vehicle(Id id,
                 const std::optional<Location>& start,
                 const std::optional<Location>& end,
                 const Amount& capacity,
                 const Skills& skills,
                 const TimeWindow& tw,
                 const std::vector<Break>& breaks,
                 const std::string& description,
                 std::vector<InputStep>&& input_steps)
  : id(id),
    start(start),
    end(end),
    capacity(capacity),
    skills(skills),
    tw(tw),
    breaks(breaks),
    description(description),
    input_steps(std::move(input_steps)) {
  if (!static_cast<bool>(start) and !static_cast<bool>(end)) {
    throw Exception(ERROR::INPUT,
                    "No start or end specified for vehicle " +
                      std::to_string(id) + '.');
  }

  for (unsigned i = 0; i < breaks.size(); ++i) {
    const auto& b = breaks[i];
    if (break_id_to_rank.find(b.id) != break_id_to_rank.end()) {
      throw Exception(ERROR::INPUT,
                      "Duplicate break id: " + std::to_string(b.id) + ".");
    }
    break_id_to_rank[b.id] = i;
  }
}

bool Vehicle::has_start() const {
  return static_cast<bool>(start);
}

bool Vehicle::has_end() const {
  return static_cast<bool>(end);
}

bool Vehicle::has_same_locations(const Vehicle& other) const {
  bool same = (this->has_start() == other.has_start()) and
              (this->has_end() == other.has_end());

  if (same and this->has_start()) {
    same = this->start.value() == other.start.value();
  }

  if (same and this->has_end()) {
    same = this->end.value() == other.end.value();
  }

  return same;
}

} // namespace vroom
