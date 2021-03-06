#ifndef SDLFIX_HPP
#define SDLFIX_HPP
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

struct SDL_Surface;
struct SDL_Renderer;

typedef struct { // This struct is not exposed by SDL API, but we need it.
  float x;
  float y;
} SDL_FPoint;

namespace AlgAudio{

/** This class provides sevaral fixes and workarounds for flaws within SDL. */
class SDLFix{
public:
  /** Mutliplies all pixel colors in a surface by alpha. */
  static void PremultiplySurface32RGBA(SDL_Surface* surf);
  /** This function substitutes several method pointers inside the renderer,
   *  so that fixed (or modified) versions of several rendering functions are
   *  used whenever SDL calls any of these methods on the renderer.
   *  It also checks whether the renderer is opengl.
   *  Generally, call this function on any fresh created renderer. */
  static void FixRenderer(SDL_Renderer* renderer);
  /** This function corrects the renderers internal blendmode state. The internal
   *  formulas SDL uses are wrong, incorrect, ugly, and do not support
   *  premultiplied alpha. Call this function on your renderer after calling
   *  SetBlendMode(BLENDMODE_BLEND), and make sure your textures use an invalid
   *  blendmode, otherwise SDL will reset */
  static void CorrectBlendMode(SDL_Renderer* renderer);
  /** Similar to SDL_RenderDrawLines, but acccepts SDL_FPoints instead of SDL_Points. */
  static void RenderDrawLines(SDL_Renderer* renderer, const SDL_FPoint* points, int count);
  /** Calls glEnable( GL_LINE_SMOOTH ). */
  static void RenderSetLineSmoothing(SDL_Renderer* renderer, bool enabled);
  /** Some SDL calls forget to make renderer's contex current, for example internal
   *  SDL_RenderSetCLipRect suffers from this problem. This function is a helpful
   *  workaround, it forces SDL to call GL_ActivateRenderer(renderer) and nothing
   *  else. */
  static void RendererMakeCurrent(SDL_Renderer* renderer);
  /** Sets GL line width. */
  static void RenderSetLineWidth(SDL_Renderer* renderer, float w);
  /*  Does some pointless GL calls to set a visible mark in gl api trace with
   *  the provided number. */
  static void MarkApiTrace(SDL_Renderer* renderer, int no);
private:
  SDLFix() = delete;
};

} // namespace AlgAudio

#endif // SDLFIX_HPP
