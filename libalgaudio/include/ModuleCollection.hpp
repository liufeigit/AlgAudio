#ifndef MODULE_CONNECTION
#define MODULE_CONNECTION
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
#include <fstream>
#include <memory>
#include <map>

#include "ModuleTemplate.hpp"
#include "Utilities.hpp"
#include "LateReturn.hpp"

namespace AlgAudio{

class LibLoader;

namespace Exceptions{
struct CollectionParse : public Exception{
  CollectionParse(std::string t) : Exception(t) {};
  CollectionParse(std::string i, std::string t) : Exception(t), id(i) {};
  virtual std::string what() override {
    if(id == "") return "While loading an unknown collection: " + text;
    else return "While loading collection '" + id + "': " + text;
  };
  std::string id = "";
};
struct CollectionLoading : public Exception{
  CollectionLoading(std::string p, std::string t) : Exception(t), path(p) {};
  virtual std::string what() override {
    return "While loading collection from '" + path + "': " + text;
  };
  std::string path;
};
} // namespace Exceptions

/** A module collection represents a set of module templates from as single
 *  module description file. */
class ModuleCollection{
public:
  ModuleCollection(std::ifstream& file, std::string basedir);
  LateReturn<> InstallAllTemplatesIntoSC();
  /** \param id The requested module template id.
   *  \returns a shared_ptr to template with the requested id, or a nullptr
   *  if no match is found. */
  std::shared_ptr<ModuleTemplate> GetTemplateByID(std::string id);
  std::map<std::string, std::shared_ptr<ModuleTemplate>> templates_by_id;
  std::string id;
  std::string name;
  std::string basedir;
  bool has_defaultlib;
  std::string defaultlib_path;
  std::shared_ptr<LibLoader> defaultlib;
};

/** ModuleCollectionBase is a set of ModuleCollections. Usually it gathers all
 *  collections loaded from all module description files. \see ModuleCollection,
 *  Module */
class ModuleCollectionBase{
private:
  ModuleCollectionBase() = delete; // static class

  static std::map<std::string, std::shared_ptr<ModuleCollection>> collections_by_id;
public:
  static std::shared_ptr<ModuleCollection> GetCollectionByID(std::string id);
  static std::shared_ptr<ModuleTemplate> GetTemplateByID(std::string id);
  static std::shared_ptr<ModuleCollection> InstallFile(std::string filepath);
  static const std::map<std::string, std::shared_ptr<ModuleCollection>>& GetCollections();
  static void InstallDir(std::string dirpath);
  static std::string ListInstalledTemplates();
  static LateReturn<> InstallAllTemplatesIntoSC();
};

} // namespace AlgAudio
#endif //MODULE_CONNECTION
