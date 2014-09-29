/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     
 * Purpose:     
 * Author:      
 * License:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "pacalib-linux.h"

#include <cairo.h>

using namespace PaCaLinux;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Surface:                                                                *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

PaCaLinux::Surface::Surface(int width, int height):
    mySurface(cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height))
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 ASSERT(cairo_surface_status(mySurface) == CAIRO_STATUS_SUCCESS, "Cairo: creating surface failed with " << GetErrorMessage(cairo_surface_status(mySurface)));
 SYS_DEBUG(DL_INFO1, "Created surface: " << getWidth() << "x" << getHeight() << " at " << mySurface);
}

PaCaLinux::Surface::~Surface()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 cairo_surface_destroy(mySurface);
 SYS_DEBUG(DL_INFO1, "Deleted surface: " << getWidth() << "x" << getHeight());
}

const char * PaCaLinux::Surface::GetErrorMessage(cairo_status_t status)
{
 ASSERT(status > 0 && status < CAIRO_STATUS_LAST_STATUS, "Cairo: status overflow (" << (int)status << ")");

 switch (status) {
   case CAIRO_STATUS_NO_MEMORY:                 return "CAIRO_STATUS_NO_MEMORY";
   case CAIRO_STATUS_INVALID_RESTORE:           return "CAIRO_STATUS_INVALID_RESTORE";
   case CAIRO_STATUS_INVALID_POP_GROUP:         return "CAIRO_STATUS_INVALID_POP_GROUP";
   case CAIRO_STATUS_NO_CURRENT_POINT:          return "CAIRO_STATUS_NO_CURRENT_POINT";
   case CAIRO_STATUS_INVALID_MATRIX:            return "CAIRO_STATUS_INVALID_MATRIX";
   case CAIRO_STATUS_INVALID_STATUS:            return "CAIRO_STATUS_INVALID_STATUS";
   case CAIRO_STATUS_NULL_POINTER:              return "CAIRO_STATUS_NULL_POINTER";
   case CAIRO_STATUS_INVALID_STRING:            return "CAIRO_STATUS_INVALID_STRING";
   case CAIRO_STATUS_INVALID_PATH_DATA:         return "CAIRO_STATUS_INVALID_PATH_DATA";
   case CAIRO_STATUS_READ_ERROR:                return "CAIRO_STATUS_READ_ERROR";
   case CAIRO_STATUS_WRITE_ERROR:               return "CAIRO_STATUS_WRITE_ERROR";
   case CAIRO_STATUS_SURFACE_FINISHED:          return "CAIRO_STATUS_SURFACE_FINISHED";
   case CAIRO_STATUS_SURFACE_TYPE_MISMATCH:     return "CAIRO_STATUS_SURFACE_TYPE_MISMATCH";
   case CAIRO_STATUS_PATTERN_TYPE_MISMATCH:     return "CAIRO_STATUS_PATTERN_TYPE_MISMATCH";
   case CAIRO_STATUS_INVALID_CONTENT:           return "CAIRO_STATUS_INVALID_CONTENT";
   case CAIRO_STATUS_INVALID_FORMAT:            return "CAIRO_STATUS_INVALID_FORMAT";
   case CAIRO_STATUS_INVALID_VISUAL:            return "CAIRO_STATUS_INVALID_VISUAL";
   case CAIRO_STATUS_FILE_NOT_FOUND:            return "CAIRO_STATUS_FILE_NOT_FOUND";
   case CAIRO_STATUS_INVALID_DASH:              return "CAIRO_STATUS_INVALID_DASH";
   case CAIRO_STATUS_INVALID_DSC_COMMENT:       return "CAIRO_STATUS_INVALID_DSC_COMMENT";
   case CAIRO_STATUS_INVALID_INDEX:             return "CAIRO_STATUS_INVALID_INDEX";
   case CAIRO_STATUS_CLIP_NOT_REPRESENTABLE:    return "CAIRO_STATUS_CLIP_NOT_REPRESENTABLE";
   case CAIRO_STATUS_TEMP_FILE_ERROR:           return "CAIRO_STATUS_TEMP_FILE_ERROR";
   case CAIRO_STATUS_INVALID_STRIDE:            return "CAIRO_STATUS_INVALID_STRIDE";
   case CAIRO_STATUS_FONT_TYPE_MISMATCH:        return "CAIRO_STATUS_FONT_TYPE_MISMATCH";
   case CAIRO_STATUS_USER_FONT_IMMUTABLE:       return "CAIRO_STATUS_USER_FONT_IMMUTABLE";
   case CAIRO_STATUS_USER_FONT_ERROR:           return "CAIRO_STATUS_USER_FONT_ERROR";
   case CAIRO_STATUS_NEGATIVE_COUNT:            return "CAIRO_STATUS_NEGATIVE_COUNT";
   case CAIRO_STATUS_INVALID_CLUSTERS:          return "CAIRO_STATUS_INVALID_CLUSTERS";
   case CAIRO_STATUS_INVALID_SLANT:             return "CAIRO_STATUS_INVALID_SLANT";
   case CAIRO_STATUS_INVALID_WEIGHT:            return "CAIRO_STATUS_INVALID_WEIGHT";
   case CAIRO_STATUS_INVALID_SIZE:              return "CAIRO_STATUS_INVALID_SIZE";
   case CAIRO_STATUS_USER_FONT_NOT_IMPLEMENTED: return "CAIRO_STATUS_USER_FONT_NOT_IMPLEMENTED";
   case CAIRO_STATUS_DEVICE_TYPE_MISMATCH:      return "CAIRO_STATUS_DEVICE_TYPE_MISMATCH";
   case CAIRO_STATUS_DEVICE_ERROR:              return "CAIRO_STATUS_DEVICE_ERROR";
   case CAIRO_STATUS_INVALID_MESH_CONSTRUCTION: return "CAIRO_STATUS_INVALID_MESH_CONSTRUCTION";
   case CAIRO_STATUS_DEVICE_FINISHED:           return "CAIRO_STATUS_DEVICE_FINISHED";
   default: break;
 }

 // Impossible event:
 return "Should not return!";
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Target:                                                                 *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

PaCaLib::TargetPtr PaCaLib::Target::Create(int width, int height)
{
 return PaCaLib::TargetPtr(new PaCaLinux::Target(width, height));
}

PaCaLinux::Target::Target(int width, int height):
    mySurface(width, height)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

}

PaCaLinux::Target::~Target()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 SYS_DEBUG(DL_INFO1, "Deleted target (" << myWidth << "x" << myHeight << ")");
}

int Target::GetHeight(void) const
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 return mySurface.getHeight();
}

int Target::GetWidth(void) const
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 return mySurface.getPhysicalWidth();
}

int Target::GetLogicalWidth(void) const
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 return mySurface.getWidth();
}

const void * Target::GetPixelData(void) const
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 return mySurface.getData();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Draw:                                                                   *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Threads::Mutex Draw::myTextMutex;

Draw::Draw(PaCaLinux::Target & target):
    target(target),
    myCairo(cairo_create(target.getSurface().get())),
    myTextOutline(0.0),
    myTextOutlineColour(0.0, 0.0, 0.0, 1.0),
    myWidth(target.GetWidth()),
    myHeight(target.GetHeight())
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 myFontDescription = pango_font_description_new();
 pango_font_description_set_family(myFontDescription, "serif");
 pango_font_description_set_weight(myFontDescription, PANGO_WEIGHT_NORMAL);
 // The font size is "2", because the output range is is -1 ... +1
 pango_font_description_set_absolute_size(myFontDescription, 2*PANGO_SCALE);

 // To be compatible with the OpenGL backend, set the coordinate ranges to -1.0 ... +1.0:
 cairo_translate(myCairo, myWidth/2, myHeight/2);
 cairo_scale(myCairo, myWidth/2, -myHeight/2);

 SYS_DEBUG(DL_INFO1, "myCairo at " << myCairo << ", font at " << myFontDescription << ", size=" << myWidth << "x" << myHeight);
}

Draw::~Draw()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 pango_font_description_free(myFontDescription);
 cairo_destroy(myCairo);
}

void Draw::Scale(float w, float h)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Scale(" << w << ", " << h << ")");
 cairo_scale(myCairo, w, h);
}

void Draw::SetLineWidth(float width)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetLineWidth(" << width << ")");
 cairo_set_line_width(myCairo, width);
}

void Draw::Move(float x, float y)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Move(" << x << ", " << y << ")");
 cairo_move_to(myCairo, x, y);
}

void Draw::Line(float x, float y)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Line(" << x << ", " << y << ")");
 cairo_line_to(myCairo, x, y);
}

void Draw::SetLineCap(PaCaLib::LineCap mode)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetLineCap(" << (int)mode << ")");
 cairo_line_cap_t linecap;
 switch (mode) {
    case PaCaLib::LINE_CAP_ROUND:
        linecap = CAIRO_LINE_CAP_ROUND;
    break;
    default:
        ASSERT(false, "Invalid LineCap: " << (int)mode);
    break;
 }
 cairo_set_line_cap(myCairo, linecap);
}

void Draw::SetColour(float r, float g, float b, float a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetColour(" << r << ", " << g << ", " << b << ", " << a << ")");
 cairo_set_source_rgba(myCairo, r, g, b, a);
}

void Draw::Rectangle(float x, float y, float w, float h)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Rectangle(" << x << ", " << y << ", " << w << ", " << h << ")");
 cairo_rectangle(myCairo, x, y, w, h);
}

void Draw::Arc(float xc, float yc, float r, float a1, float a2)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Arc(" << xc << ", " << yc << ", " << r << ", " << a1 << ", " << a2 << ")");
 cairo_arc(myCairo, xc, yc, r, a1, a2);
}

float Draw::DrawTextInternal(float x, float y, PaCaLib::TextMode mode, const char * text, float size, float offset, float aspect)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "DrawText(" << x << ", " << y << ", " << (int)mode << ", '" << text << "', " << size << ", " << aspect << ")");

 // I don't know why, the text rendering is not thread-safe. At least, locking this function resolves some
 // very strange error messages or even crashes.
 // TODO: Find a better way!
 Threads::Lock _l(myTextMutex);

 CairoSave _s(myCairo);

 SYS_DEBUG(DL_INFO1, "saved " << myCairo);

 float h = 0.70;
 float v = 0.70; // Heuristic values :-)

 h *= size * aspect;
 v *= size;

 Scale(h, v);
 SYS_DEBUG(DL_INFO1, "cairo_scale(" << h << ", " << v << ") ok");

 PangoLayout *layout = pango_cairo_create_layout(myCairo);
 ASSERT(layout, "pango_cairo_create_layout() failed");

 SYS_DEBUG(DL_INFO1, "layout=" << layout);

 pango_layout_set_text(layout, text, -1);
 SYS_DEBUG(DL_INFO1, "pango_layout_set_text() ok");
 pango_layout_set_font_description(layout, myFontDescription);
 SYS_DEBUG(DL_INFO1, "pango_layout_set_font_description() ok");

 NewPath();
 SYS_DEBUG(DL_INFO1, "cairo_new_path() ok");

 pango_cairo_update_layout(myCairo, layout);
 SYS_DEBUG(DL_INFO1, "pango_cairo_update_layout() ok");
 int width, height;
 pango_layout_get_size(layout, &width, &height);
 SYS_DEBUG(DL_INFO1, "pango_layout_get_size(): w=" << width << ", h=" << height);
 float text_width_half = (float)width / (2*PANGO_SCALE);
 float text_height_half = (float)height / (2*PANGO_SCALE);

 float x_pos = x/h - text_width_half;
 float y_pos = y/v - text_height_half - (float)height * offset / PANGO_SCALE;

 switch (mode) {
    case PaCaLib::LEFT:
        x_pos += text_width_half;
    break;
    case PaCaLib::CENTER:
        // Nothing to do
    break;
    case PaCaLib::RIGHT:
        x_pos -= text_width_half;
    break;
 }
 Move(x_pos, y_pos);
 SYS_DEBUG(DL_INFO1, "cairo_move_to() ok");
 pango_cairo_layout_path(myCairo, layout);
 SYS_DEBUG(DL_INFO1, "pango_cairo_layout_path() ok");

 if (myTextOutline > 0.0) {
    SetLineWidth(text_height_half * myTextOutline);
    cairo_stroke(myCairo);
 } else if (myTextOutline < 0.0) {
    cairo_fill_preserve(myCairo);
    SetLineWidth(-text_height_half * myTextOutline);
    PaCaLib::Draw::SetColour(myTextOutlineColour);
    cairo_stroke(myCairo);
 } else {
    cairo_fill(myCairo);
 }
 SYS_DEBUG(DL_INFO1, "draw ok");

 g_object_unref(layout);
 SYS_DEBUG(DL_INFO1, "unref ok");

 return (2.0 * h) * text_width_half;
}

void Draw::SetTextOutlineColour(float r, float g, float b, float a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetTextOutlineColour(" << r << ", " << g << ", " << b << ", " << a << ")");
 myTextOutlineColour = PaCaLib::Colour(r, g, b, a);
}

void Draw::SetTextOutline(float outline)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetTextOutline(" << outline << ")");
 myTextOutline = outline;
}

void Draw::Paint(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Paint()");
 cairo_paint(myCairo);
}

void Draw::Paint(float alpha)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Paint(" << alpha << ")");
 cairo_paint_with_alpha(myCairo, alpha);
}

void Draw::Operator(PaCaLib::Oper op)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Operator(" << (int)op << ")");

 cairo_operator_t cairo_op;

 switch (op) {
    case PaCaLib::OP_SOURCE:
        cairo_op = CAIRO_OPERATOR_SOURCE;
    break;
    case PaCaLib::OP_OVER:
        cairo_op = CAIRO_OPERATOR_DEST_OVER;
    break;
    default:
        ASSERT(false, "Invalid operator: " << (int)op);
    break;
 }

 cairo_set_operator(myCairo, cairo_op);
}

PathPtr Draw::NewPath(void)
{
 return PathPtr(new Path(*this));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Path:                                                                   *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Path::Path(Draw & parent):
    parent(parent)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 Clear();
}

Path::~Path()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

void Path::Move(float x, float y)
{
 parent.Move(x, y);
}

void Path::Line(float x, float y)
{
 parent.Line(x, y);
}

void Path::Arc(float xc, float yc, float r, float a1, float a2)
{
 parent.Arc(xc, yc, r, a1, a2);
}

void Path::Close(void)
{
 cairo_close_path(parent.myCairo);
}

void Path::Clear(void)
{
 cairo_new_path(parent.myCairo);
}

void Path::SetLineWidth(float width)
{
 parent.SetLineWidth(width);
}

void Path::SetLineCap(PaCaLib::LineCap mode)
{
 parent.SetLineCap(mode);
}

void Path::Fill(void)
{
 cairo_fill_preserve(parent.myCairo);
}

void Path::Stroke(void)
{
 cairo_stroke(parent.myCairo);
}

void Path::SetColour(float r, float g, float b, float a)
{
 parent.SetColour(r, g, b, a);
}

PaCaLib::DrawPtr Target::Draw(void)
{
 return PaCaLib::DrawPtr(new PaCaLinux::Draw(*this));
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
