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

#include "UI/UITextEntry.hpp"
#include "TextRenderer.hpp"
#include "SDLMain.hpp"

namespace AlgAudio{

UITextEntry::UITextEntry(std::weak_ptr<Window> w, std::string t) : UIWidget(w), text(t) {
}

std::shared_ptr<UITextEntry> UITextEntry::Create(std::weak_ptr<Window> parent_window, std::string t){
  auto res = std::shared_ptr<UITextEntry>(new UITextEntry(parent_window, t));
  res->Init();
  return res;
}

void UITextEntry::Init(){
  SetMinimalSize(Size2D(10, fontsize + 2));
  UpdateText();
}

void UITextEntry::CustomDraw(DrawContext& c){
  Color bg_color = Theme::Get("textentry-bg");
  bool focused = IsFocused();
  if(focused) bg_color = bg_color.Lighter(0.1);
  c.SetColor(bg_color);
  c.DrawRect(Rect(Point2D(0,0), c.Size()));

  c.DrawText(text_texture, Theme::Get( (text!="" || focused) ? "textentry-text" : "textentry-default"), Point2D( c.Size().height/2.0f - text_texture->GetSize().height/2.0f + 1.0f ,0));
  
  if(focused){
    c.SetColor(Theme::Get("textentry-default"));
    c.DrawLine(Point2D(text_texture->GetSize().width + 2, 2),
               Point2D(text_texture->GetSize().width + 2, c.Size().height - 2)
              );
  }
}

void UITextEntry::SetText(std::string t){
  text = t;
  if(max_length != -1) text = text.substr(0,max_length);
  UpdateText();
}

void UITextEntry::OnKeyboard(KeyData k){
  if(k.pressed == false) return; // Ignore key up events
  if( k.type == KeyData::KeyType::Text ){
    if(digits_only){
      if(std::isdigit(k.symbol[0])){
        // okay.
      }else if(k.symbol[0] == '-'){
        // TODO: only allowed as the first character
      }else if(k.symbol[0] == '.'){
        // TODO: only allowed once
      }else{
        // disallowed character
        return;
      }
    }
    
    if(max_length == -1 || text.length() < (unsigned int)max_length){
      text += k.symbol;
      on_edited.Happen();
      was_edited_since_received_focus = true;
      UpdateText();
    }
  }else if(k.type == KeyData::KeyType::Backspace){
    if(text.length() >= 1){
      text.pop_back();
      on_edited.Happen();
      was_edited_since_received_focus = true;
      UpdateText();
    }
  }else if(k.type == KeyData::KeyType::Return){
    on_edit_complete.Happen();
  }
}

void UITextEntry::OnFocusChanged(bool has_focus){
  if(has_focus){
    SDLMain::SetTextInput(true);
    was_edited_since_received_focus = false;
  }else{
    SDLMain::SetTextInput(false);
    if(was_edited_since_received_focus){
      on_edit_exitted.Happen();
    }
  }
  if(text == "") UpdateText();
  SetNeedsRedrawing();
}

void UITextEntry::SetFontSize(int size){
  fontsize = size;
  SetMinimalSize(Size2D(10, fontsize + 2));
  UpdateText();
}

void UITextEntry::UpdateText(){
  text_texture = TextRenderer::Render(window, FontParams("Dosis-Regular", fontsize), (text!="" || IsFocused()) ? text : default_text);
  SetNeedsRedrawing();
}

void UITextEntry::SetMaxLength(int l){
  max_length = l;
  if(max_length != -1){
    text = text.substr(0,max_length);
    UpdateText();
  }
}

} // namespace AlgAudio
