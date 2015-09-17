#ifndef CONFIG_HPP
#define CONFIG_HPP
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

namespace AlgAudio{

class Config{
public:
	bool use_sc;
	bool supernova;
	bool debug;
	bool debug_osc;
	std::string path_to_sclang;
	std::string scsynth_audio_driver_name;
	
	Config(const Config& other) = default;
	
	static Config& Global();
	static Config Default();
	
	// TODO: Store/load to/from file
	
private:
	Config() {};
	static std::unique_ptr<Config> global;
};

} // namespace AlgAudio

#endif // CONFIG_HPP
