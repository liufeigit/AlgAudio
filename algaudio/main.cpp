#include <iostream>
#include "ModuleCollection.hpp"
#include "ModuleFactory.hpp"
#include "SDLMain.hpp"
#include "UI/UIButton.hpp"
#include "UI/UIMarginBox.hpp"

using namespace AlgAudio;

int main(){
  try{
    ModuleCollectionBase::InstallDir("modules");
    std::cout << ModuleCollectionBase::ListInstalledTemplates();
    auto module1 = ModuleFactory::CreateNewInstance("debug/helloworld");
    auto module2 = ModuleFactory::CreateNewInstance("debug/pipe");

    auto blah_window = std::make_shared<UIWindow>("BLAH");
    auto marginbox = std::make_shared<UIMarginBox>(100,50,20,5);
    auto button = std::make_shared<UIButton>("Button");
    blah_window->Insert(marginbox);
    marginbox->Insert(button);

    SDLMain::RegisterWindow(blah_window);
    SDLMain::Run();

  }catch(Exception ex){
    std::cout << "An unhandled exception occured: " << ex.what() << std::endl;
  }
}
