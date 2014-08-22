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

Threads::Mutex Target::myTextMutex;

PaCaLinux::Target::Target(int width, int height):
    myWidth(width),
    myHeight(height),
    mySurface(width, height),
    myCairo(cairo_create(mySurface.get())),
    myTextOutline(0.0),
    myTextOutlineColour(0.0, 0.0, 0.0, 1.0)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 myFontDescription = pango_font_description_new();
 pango_font_description_set_family(myFontDescription, "serif");
 pango_font_description_set_weight(myFontDescription, PANGO_WEIGHT_NORMAL);
 // The font size is "2", because the output range is is -1 ... +1
 pango_font_description_set_absolute_size(myFontDescription, 2*PANGO_SCALE);

 // To be compatible with the OpenGL backend, set the coordinate ranges to -1.0 ... +1.0:
 cairo_translate(myCairo, myWidth/2, myHeight/2);
 Scale(myWidth/2, -myHeight/2);

 SYS_DEBUG(DL_INFO1, "myCairo at " << myCairo << ", font at " << myFontDescription << ", size=" << myWidth << "x" << myHeight);
}

PaCaLinux::Target::~Target()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 pango_font_description_free(myFontDescription);
 cairo_destroy(myCairo);

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

void Target::Scale(double w, double h)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Scale(" << w << ", " << h << ")");
 cairo_scale(myCairo, w, h);
}

void Target::Stroke(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Stroke()");
 cairo_stroke(myCairo);
}

void Target::Fill(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Fill()");
 cairo_fill(myCairo);
}

void Target::FillPreserve(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "FillPreserve()");
 cairo_fill_preserve(myCairo);
}

void Target::SetLineWidth(double width)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetLineWidth(" << width << ")");
 cairo_set_line_width(myCairo, width);
}

void Target::Move(double x, double y)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Move(" << x << ", " << y << ")");
 cairo_move_to(myCairo, x, y);
}

void Target::Line(double x, double y)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Line(" << x << ", " << y << ")");
 cairo_line_to(myCairo, x, y);
}

void Target::SetLineCap(PaCaLib::LineCap mode)
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

void Target::SetColour(double r, double g, double b)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetColour(" << r << ", " << g << ", " << b << ")");
 cairo_set_source_rgb(myCairo, r, g, b);
}

void Target::SetColour(double r, double g, double b, double a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetColour(" << r << ", " << g << ", " << b << ", " << a << ")");
 cairo_set_source_rgba(myCairo, r, g, b, a);
}

void Target::SetColour(const PaCaLib::Colour & col)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetColour(" << col << ")");
 cairo_set_source_rgba(myCairo, col.r, col.g, col.b, col.a);
}

void Target::Rectangle(double x, double y, double w, double h)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Rectangle(" << x << ", " << y << ", " << w << ", " << h << ")");
 cairo_rectangle(myCairo, x, y, w, h);
}

void Target::Arc(double xc, double yc, double r, double a1, double a2)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Arc(" << xc << ", " << yc << ", " << r << ", " << a1 << ", " << a2 << ")");
 cairo_arc(myCairo, xc, yc, r, a1, a2);
}

void Target::NewPath(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "NewPath()");
 cairo_new_path(myCairo);
}

void Target::NewSubPath(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "NewSubPath()");
 cairo_new_sub_path(myCairo);
}

void Target::ClosePath(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "ClosePath()");
 cairo_close_path(myCairo);
}

double Target::DrawText(double x, double y, PaCaLib::TextMode mode, const char * text, double size, double aspect)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "DrawText(" << x << ", " << y << ", " << (int)mode << ", '" << text << "', " << size << ", " << aspect << ")");

 Threads::Lock _l(myTextMutex);

 cairo_save(myCairo);

 SYS_DEBUG(DL_INFO1, "saved " << myCairo);

 double h = 0.70;
 double v = 0.70; // Heuristic values :-)

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
 double text_width_half = (double)width / (2*PANGO_SCALE);
 double text_height_half = (double)height / (2*PANGO_SCALE);

 double x_pos = x/h - text_width_half;
 double y_pos = y/v - text_height_half;

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
    Stroke();
 } else if (myTextOutline < 0.0) {
    FillPreserve();
    SetLineWidth(-text_height_half * myTextOutline);
    SetColour(myTextOutlineColour);
    Stroke();
 } else {
    Fill();
 }
 SYS_DEBUG(DL_INFO1, "draw ok");

 g_object_unref(layout);
 SYS_DEBUG(DL_INFO1, "unref ok");

 cairo_restore(myCairo);
 SYS_DEBUG(DL_INFO1, "cairo_restore() ok");

 return (2.0 * h) * text_width_half;
}

void Target::SetTextOutlineColour(double r, double g, double b, double a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetTextOutlineColour(" << r << ", " << g << ", " << b << ", " << a << ")");
 myTextOutlineColour = PaCaLib::Colour(r, g, b, a);
}

void Target::SetTextOutline(double outline)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetTextOutline(" << outline << ")");
 myTextOutline = outline;
}

void Target::Paint(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Paint()");
 cairo_paint(myCairo);
}

void Target::Paint(double alpha)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Paint(" << alpha << ")");
 cairo_paint_with_alpha(myCairo, alpha);
}

void Target::Operator(PaCaLib::Oper op)
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

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
