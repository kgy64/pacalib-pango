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
    myOutlineWidth(0.0),
    myOutlineColour(0.0, 0.0, 0.0, 1.0),
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

 static_cast<PaCaLib::Draw &>(*this).SetColourCompose();

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

void Draw::SetColourCompose(PaCaLib::ColourCompose mode)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetColourCompose(" << mode << ")");

 cairo_operator_t op = CAIRO_OPERATOR_SOURCE;

 switch (mode) {
    case PaCaLib::COLOUR_COMPOSE_DEFAULT:
        // nothing to do
    break;
    case PaCaLib::COLOUR_COMPOSE_ADD:
        op = CAIRO_OPERATOR_ADD;
    break;
    case PaCaLib::COLOUR_COMPOSE_SUBTRACT:
        op = CAIRO_OPERATOR_OVER;
    break;
    case PaCaLib::COLOUR_COMPOSE_OVERWRITE:
        op = CAIRO_OPERATOR_SOURCE;
    break;
    default:
        ASSERT(false, "using " << mode << " is not supported");
    break;
 }

 cairo_set_operator(getCairo(), op);
}

void Draw::SetOutlineColour(float r, float g, float b, float a)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetOutlineColour(" << r << ", " << g << ", " << b << ", " << a << ")");
 myOutlineColour = PaCaLib::Colour(r, g, b, a);
}

void Draw::SetOutlineWidth(float outline)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "SetOutlineWidth(" << outline << ")");
 myOutlineWidth = outline;
}

float Draw::DrawTextInternal(const PaCaLib::Draw::TextParams & params, const PaCaLib::Draw::Distortion * distortion)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "DrawTextInternal(" << params);

 // I don't know why, the text rendering is not thread-safe. At least, locking this function resolves some
 // very strange error messages or even crashes.
 // TODO: Find a better way!
 Threads::Lock _l(myTextMutex);

 CairoSave _s(getCairo());

 float h = 0.75;
 float v = 0.75; // Heuristic values :-)

 h *= params.size;
 v *= params.size;

 cairo_scale(getCairo(), h, v);
 SYS_DEBUG(DL_INFO1, "cairo_scale(" << h << ", " << v << ") ok");

 float x = params.x / h;
 float y = params.y / v;

 // x:y is now the reference point of the text on the target

 PangoLayout *layout = pango_cairo_create_layout(getCairo());
 ASSERT(layout, "pango_cairo_create_layout() failed");

 SYS_DEBUG(DL_INFO1, "layout=" << layout);

 pango_layout_set_text(layout, params.text, -1);
 SYS_DEBUG(DL_INFO1, "pango_layout_set_text() ok");
 pango_layout_set_font_description(layout, myFontDescription);
 SYS_DEBUG(DL_INFO1, "pango_layout_set_font_description() ok");

 cairo_new_path(getCairo());
 SYS_DEBUG(DL_INFO1, "cairo_new_path() ok");

 pango_cairo_update_layout(getCairo(), layout);
 SYS_DEBUG(DL_INFO1, "pango_cairo_update_layout() ok");
 int width, height;
 pango_layout_get_size(layout, &width, &height);
 width *= params.aspect;
 SYS_DEBUG(DL_INFO1, "pango_layout_get_size(): w=" << width << ", h=" << height);
 float text_width_half = (float)width / (2*PANGO_SCALE);
 float text_height_half = (float)height / (2*PANGO_SCALE);

 float xc = -text_width_half;
 float yc = (float)height * params.offset / (float)(-PANGO_SCALE);

 switch (params.mode) {
    case PaCaLib::LEFT:
        xc = 0.0f;
    break;
    case PaCaLib::CENTER:
        // Nothing to do here
    break;
    case PaCaLib::RIGHT:
        xc -= text_width_half;
    break;
 }

 cairo_translate(getCairo(), x, y);
 SYS_DEBUG(DL_INFO1, "cairo_translate(" << x << ", " << y << ") ok");

 if (distortion) {
    if (distortion->rotation != 0.0f) {
        SYS_DEBUG(DL_INFO1, "scene rotation by " << distortion.rotation);
        cairo_rotate(getCairo(), distortion->rotation);
    }

    if (distortion->obj_size != 1.0f) {
        cairo_matrix_t m = {
            distortion->obj_size,   0.0,
            0.0,                distortion->obj_size,
            0.0,                0.0
        };
        cairo_transform(getCairo(), &m);
    }

    if (distortion->shear_x != 0.0f) {
        cairo_matrix_t m = {
            1.0,                    0.0,
            -distortion->shear_x,   1.0,
            0.0,                    0.0
        };
        cairo_transform(getCairo(), &m);
    }

    if (distortion->shear_y != 0.0f) {
        cairo_matrix_t m = {
            1.0,    -distortion->shear_y,
            0.0,    1.0,
            0.0,    0.0
        };
        cairo_transform(getCairo(), &m);
    }

    if (distortion->scene_height != 1.0f) {
        cairo_matrix_t m = {
            1.0,    0.0,
            0.0,    distortion->scene_height,
            0.0,    0.0
        };
        cairo_transform(getCairo(), &m);
    }

 }

 if (params.rotation != 0.0f) {
    SYS_DEBUG(DL_INFO1, "rotation by " << params.rotation);
    cairo_rotate(getCairo(), params.rotation);
 }

 if (distortion) {
    if (distortion->obj_height != 1.0f) {
        cairo_matrix_t m = {
            1.0,    0.0,
            0.0,    distortion->obj_height,
            0.0,    0.0
        };
        cairo_transform(getCairo(), &m);
    }
 }

 if (params.shear_x != 0.0f) {
    cairo_matrix_t m = {
        1.0,                0.0,
        -params.shear_x,    1.0,
        0.0,                0.0
    };
    cairo_transform(getCairo(), &m);
 }

 if (params.shear_y != 0.0f) {
    cairo_matrix_t m = {
        1.0,        -params.shear_y,
        0.0,        1.0,
        0.0,        0.0
    };
    cairo_transform(getCairo(), &m);
 }

 yc -= text_height_half;

 cairo_move_to(getCairo(), xc, yc);
 SYS_DEBUG(DL_INFO1, "cairo_move_to(" << xc << ", " << yc << ")");

 if (params.aspect != 1.0f) {
    cairo_scale(getCairo(), params.aspect, 1.0);
 }

 pango_cairo_layout_path(getCairo(), layout);
 SYS_DEBUG(DL_INFO1, "pango_cairo_layout_path() ok");

 if (myOutlineWidth < 0.0) {
    SetLineWidth(text_height_half * -myOutlineWidth);
    cairo_stroke(getCairo());
 } else if (myOutlineWidth > 0.0) {
    cairo_fill_preserve(getCairo());
    SetLineWidth(text_height_half * myOutlineWidth);
    PaCaLib::Draw::SetColour(myOutlineColour);
    cairo_stroke(getCairo());
 } else {
    cairo_fill(getCairo());
 }
 SYS_DEBUG(DL_INFO1, "draw ok");

 g_object_unref(layout);
 SYS_DEBUG(DL_INFO1, "unref ok");

 return 2.0f * h * text_width_half;
}

void Draw::Paint(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);
 SYS_DEBUG(DL_INFO1, "Paint()");

 cairo_paint(getCairo());
}

PathPtr Draw::NewPath(void)
{
 return PathPtr(new Path(*this));
}

void Draw::DrawPath(PaCaLib::Path::DrawMode mode)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 switch (mode) {
    case PaCaLib::Path::DRAW_STROKE:
        SYS_DEBUG(DL_INFO1, "Path is drawn with \"stroke\".");
        cairo_stroke(getCairo());
    break;
    case PaCaLib::Path::DRAW_FILL:
        SYS_DEBUG(DL_INFO1, "Path is drawn with \"fill\".");
        cairo_fill(getCairo());
    break;
    case PaCaLib::Path::DRAW_STROKE_AND_FILL:
        SYS_DEBUG(DL_INFO1, "Path is drawn with \"stroke and fill\".");
        cairo_fill_preserve(getCairo());
        PaCaLib::Draw::SetColour(myOutlineColour);
        cairo_stroke(getCairo());
    break;
    default:
        SYS_DEBUG(DL_INFO2, "Path is not drawn.");
    break;
 }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\
 *                                                                                       *
 *         class Path:                                                                   *
 *                                                                                       *
\* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Path::Path(PaCaLinux::Draw & parent):
    parent(parent),
    is_bezier(false),
    bezier_dx(0.0f),
    bezier_dy(0.0f)
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
 SYS_DEBUG_MEMBER(DM_PACALIB);

 cairo_move_to(parent.getCairo(), x, y);
}

void Path::Line(float x, float y)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 cairo_line_to(parent.getCairo(), x, y);
}

void Path::Arc(float xc, float yc, float r, float a1, float a2)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 cairo_arc(parent.getCairo(), xc, yc, r, a1, a2);
}

void Path::Bezier(float x, float y, float dx, float dy)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 if (is_bezier) {
    double px, py;
    cairo_get_current_point(parent.getCairo(), &px, &py);
    px += bezier_dx;
    py += bezier_dy;
    cairo_curve_to(parent.getCairo(), px, py, x - dx, y - dy, x, y);
 } else {
    cairo_move_to(parent.getCairo(), x, y);
    is_bezier = true;
 }

 bezier_dx = dx;
 bezier_dy = dy;
}

void Path::Close(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 cairo_close_path(parent.getCairo());
}

void Path::Clear(void)
{
 SYS_DEBUG_MEMBER(DM_PACALIB);

 cairo_new_path(parent.getCairo());
 is_bezier = false;
}

void Path::Draw(PaCaLib::Path::DrawMode mode)
{
 parent.DrawPath(mode);
}

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
