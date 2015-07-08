#include "UI/UIMarginBox.hpp"

namespace AlgAudio{

UIMarginBox::UIMarginBox(int t, int r, int b, int l) :
  top(t), right(r), bottom(b), left(l)
{ }

std::shared_ptr<UIMarginBox> UIMarginBox::Create(int t, int r, int b, int l){
  std::shared_ptr<UIMarginBox> res(new UIMarginBox(t,r,b,l));
  return res;
}

void UIMarginBox::CustomDraw(const DrawContext& c){
  DrawContext c2 = c.SubContext(left, top, c.width - right - left, c.height - top - bottom);
  if(!c2.HasZeroArea() && child)
    child->Draw(c2);
}

void UIMarginBox::Insert(std::shared_ptr<UIWidget> ch){
  child = ch;
  child->window = window;
  child->parent = shared_from_this();
}

void UIMarginBox::OnMouseButton(bool down, short b,int x,int y){
  if(!child) return;
  if(x < left || y < top || x > last_drawn_size.width - right || y > last_drawn_size.height - bottom) return;
  child->OnMouseButton(down,b,x-top,y-left);
}

bool UIMarginBox::IsInside(int x, int y) const{
  if(x < left || y < top || x > last_drawn_size.width - right || y > last_drawn_size.height - bottom) return false;
  else return true;
}

void UIMarginBox::OnMotion(int x1, int y1, int x2, int y2){
  if(!child) return;
  if(IsInside(x1,y1) && IsInside(x2,y2)) child->OnMotion(x1,y1,x2,y2);
  else if(IsInside(x1,y1)){
    // start inside, end ouside
    child->OnMotionLeave();
  }else if(IsInside(x2,y2)){
    // start outside, enter outside
    child->OnMotionEnter();
  }else{
    // both outside, ignore
  }
}

void UIMarginBox::OnMotionEnter(){

}
void UIMarginBox::OnMotionLeave(){
  if(child)
    child->OnMotionLeave();
}


} // namespace AlgAudio
