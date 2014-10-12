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

Surface::Surface(int width, int height, Glesly::PixelFormat format):
    myPixelFormat(format),
    mySurface(cairo_image_surface_create(GetCairoPixelFormat(), width, height))
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 ASSERT(cairo_surface_status(mySurface) == CAIRO_STATUS_SUCCESS, "creating Cairo Surface failed with " << GetErrorMessage(cairo_surface_status(mySurface)));
 SYS_DEBUG(DL_INFO1, "Created surface: " << getWidth() << "x" << getHeight() << " at " << mySurface);
}

Surface::Surface(const Glesly::Target2D & source):
    myPixelFormat(source.GetPixelFormat()),
    mySurface(nullptr)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 cairo_format_t format = GetCairoPixelFormat(myPixelFormat);
 ASSERT(format != CAIRO_FORMAT_INVALID, "trying to copy from source with unknown pixel format");

 int stride = cairo_format_stride_for_width(format, source.GetWidth());
 unsigned char * pixeldata = reinterpret_cast<unsigned char *>(const_cast<void *>(source.GetPixelData()));
 mySurface = cairo_image_surface_create_for_data(pixeldata, format, source.GetWidth(), source.GetHeight(), stride);

 ASSERT(cairo_surface_status(mySurface) == CAIRO_STATUS_SUCCESS, "could not create cairo surface");
}

Surface::~Surface()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 cairo_surface_destroy(mySurface);
 SYS_DEBUG(DL_INFO1, "Deleted surface: " << getWidth() << "x" << getHeight());
}

const char * PaCaLinux::GetErrorMessage(cairo_status_t status)
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

PaCaLib::TargetPtr PaCaLib::Target::Create(int width, int height, Glesly::PixelFormat format)
{
 if (format == Glesly::FORMAT_DEFAULT) {
    format = Glesly::FORMAT_RGBA_8888;
 }

 return PaCaLib::TargetPtr(new PaCaLinux::Target(width, height, format));
}

Target::Target(int width, int height, Glesly::PixelFormat format):
    mySurface(width, height, format)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
}

Target::~Target()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 SYS_DEBUG(DL_INFO1, "Deleted target (" << myWidth << "x" << myHeight << ")");
}

Glesly::Target2D & Target::operator=(const Glesly::Target2D & other)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 cairo_t * cairo = cairo_create(getSurface().get());
 cairo_set_source_surface(cairo, Surface(other).get(), 0.0, 0.0);
 cairo_paint(cairo);
 cairo_destroy(cairo);

 return *this;
}

int Target::GetHeight(void) const
{
 return mySurface.getHeight();
}

int Target::GetWidth(void) const
{
 return mySurface.getPhysicalWidth();
}

int Target::GetLogicalWidth(void) const
{
 return mySurface.getWidth();
}

const void * Target::GetPixelData(void) const
{
 return mySurface.getData();
}

Glesly::PixelFormat Target::GetPixelFormat(void) const
{
 return mySurface.GetPixelFormat();
}

PaCaLib::DrawPtr Target::Draw(void)
{
 return PaCaLib::DrawPtr(new PaCaLinux::Draw(*this));
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

 ASSERT(cairo_status(getCairo()) == CAIRO_STATUS_SUCCESS, "creating Cairo failed with " << GetErrorMessage(cairo_status(getCairo())));

 myFontDescription = pango_font_description_new();
 pango_font_description_set_family(myFontDescription, "serif");
 pango_font_description_set_weight(myFontDescription, PANGO_WEIGHT_NORMAL);
 // The font size is "2", because the output range is is -1 ... +1
 pango_font_description_set_absolute_size(myFontDescription, 2*PANGO_SCALE);

 // To be compatible with the OpenGL backend, set the coordinate ranges to -1.0 ... +1.0:
 cairo_translate(getCairo(), myWidth/2, myHeight/2);
 cairo_scale(getCairo(), myWidth/2, -myHeight/2);

 SYS_DEBUG(DL_INFO1, "myCairo at " << getCairo() << ", font at " << myFontDescription << ", size=" << myWidth << "x" << myHeight);
}

Draw::~Draw()
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 pango_font_description_free(myFontDescription);
 cairo_destroy(getCairo());
}

void Draw::Scale(float w, float h)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Scale(" << w << ", " << h << ")");
 cairo_scale(getCairo(), w, h);
}

void Draw::SetLineWidth(float width)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetLineWidth(" << width << ")");
 cairo_set_line_width(getCairo(), width);
}

void Draw::SetLineCap(PaCaLib::LineCap mode)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetLineCap(" << (int)mode << ")");

 cairo_line_cap_t linecap;

 switch (mode) {
    case PaCaLib::LINE_CAP_NONE:
        linecap = CAIRO_LINE_CAP_BUTT;
    break;
    case PaCaLib::LINE_CAP_ROUND:
        linecap = CAIRO_LINE_CAP_ROUND;
    break;
    case PaCaLib::LINE_CAP_SQUARE:
        linecap = CAIRO_LINE_CAP_SQUARE;
    break;
    default:
        ASSERT(false, "Invalid LineCap: " << (int)mode);
    break;
 }

 cairo_set_line_cap(getCairo(), linecap);
}

void Draw::SetColour(float r, float g, float b, float a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetColour(" << r << ", " << g << ", " << b << ", " << a << ")");
 cairo_set_source_rgba(getCairo(), r, g, b, a);
}

float Draw::DrawTextInternal(float x, float y, PaCaLib::TextMode mode, const char * text, float size, float offset, float aspect)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "DrawText(" << x << ", " << y << ", " << (int)mode << ", '" << text << "', " << size << ", " << aspect << ")");

 // I don't know why, the text rendering is not thread-safe. At least, locking this function resolves some
 // very strange error messages or even crashes.
 // TODO: Find a better way!
 Threads::Lock _l(myTextMutex);

 CairoSave _s(getCairo());

 float h = 0.75;
 float v = 0.75; // Heuristic values :-)

 h *= size * aspect;
 v *= size;

 Scale(h, v);
 SYS_DEBUG(DL_INFO1, "cairo_scale(" << h << ", " << v << ") ok");

 PangoLayout *layout = pango_cairo_create_layout(getCairo());
 ASSERT(layout, "pango_cairo_create_layout() failed");

 SYS_DEBUG(DL_INFO1, "layout=" << layout);

 pango_layout_set_text(layout, text, -1);
 SYS_DEBUG(DL_INFO1, "pango_layout_set_text() ok");
 pango_layout_set_font_description(layout, myFontDescription);
 SYS_DEBUG(DL_INFO1, "pango_layout_set_font_description() ok");

 NewPath();
 SYS_DEBUG(DL_INFO1, "cairo_new_path() ok");

 pango_cairo_update_layout(getCairo(), layout);
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
 cairo_move_to(getCairo(), x_pos, y_pos);
 SYS_DEBUG(DL_INFO1, "cairo_move_to() ok");
 pango_cairo_layout_path(getCairo(), layout);
 SYS_DEBUG(DL_INFO1, "pango_cairo_layout_path() ok");

 cairo_set_operator(getCairo(), CAIRO_OPERATOR_ADD);

 if (myTextOutline < 0.0) {
    SetLineWidth(-text_height_half * myTextOutline);
    cairo_stroke(getCairo());
 } else if (myTextOutline > 0.0) {
    cairo_fill_preserve(getCairo());
    SetLineWidth(text_height_half * myTextOutline);
    PaCaLib::Draw::SetColour(myTextOutlineColour);
    cairo_stroke(getCairo());
 } else {
    cairo_fill(getCairo());
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
 cairo_set_operator(getCairo(), CAIRO_OPERATOR_SOURCE);
 cairo_paint(getCairo());
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

 Clear();
}

void Path::Move(float x, float y)
{
 cairo_move_to(parent.getCairo(), x, y);
}

void Path::Line(float x, float y)
{
 cairo_line_to(parent.getCairo(), x, y);
}

void Path::Arc(float xc, float yc, float r, float a1, float a2)
{
 cairo_arc(parent.getCairo(), xc, yc, r, a1, a2);
}

void Path::Close(void)
{
 cairo_close_path(parent.getCairo());
}

void Path::Clear(void)
{
 cairo_new_path(parent.getCairo());
}

void Path::Stroke(void)
{
 cairo_set_operator(parent.getCairo(), CAIRO_OPERATOR_ADD);
 cairo_stroke_preserve(parent.getCairo());
}

void Path::Fill(void)
{
 cairo_set_operator(parent.getCairo(), CAIRO_OPERATOR_ADD);
 cairo_fill_preserve(parent.getCairo());
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
