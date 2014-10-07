/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * Project:     
 * Purpose:     
 * Author:      
 * License:     GPL (see file 'COPYING' in the project root for more details)
 * Comments:    
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef __SRC_PACALIB_LINUX_H_INCLUDED__
#define __SRC_PACALIB_LINUX_H_INCLUDED__

#include <Threads/Mutex.h>
#include <glesly/format.h>
#include <pacalib/pacalib.h>
#include <Debug/Debug.h>

#include <pango/pangocairo.h>

namespace PaCaLinux
{
    inline cairo_format_t ConvertCairoPixelFormat(Glesly::PixelFormat format)
    {
        switch (format) {
            case Glesly::FORMAT_RGB_565:
                return CAIRO_FORMAT_RGB16_565;
            break;
            case Glesly::FORMAT_RGB_888:
                return CAIRO_FORMAT_RGB24;
            break;
            case Glesly::FORMAT_RGBA_8888:
                return CAIRO_FORMAT_ARGB32;
            break;
            default:
            break;
        }
        return CAIRO_FORMAT_INVALID;
    }

    const char * GetErrorMessage(cairo_status_t status);

    class CairoSave
    {
     public:
        inline CairoSave(cairo_t * cairo):
            cairo(cairo)
        {
            cairo_save(cairo);
        }

        inline ~CairoSave()
        {
            cairo_restore(cairo);
        }

     private:
        cairo_t * cairo;

    }; // class PaCaLinux::CairoSave

    class Surface
    {
     public:
        Surface(int width, int height, Glesly::PixelFormat format);
        VIRTUAL_IF_DEBUG ~Surface();

        inline cairo_surface_t * get(void)
        {
            return mySurface;
        }

        inline const cairo_surface_t * get(void) const
        {
            return mySurface;
        }

        inline void * getData(void)
        {
            return cairo_image_surface_get_data(get());
        }

        inline const void * getData(void) const
        {
            return cairo_image_surface_get_data(const_cast<cairo_surface_t*>(get()));
        }

        inline int getWidth(void) const
        {
            return cairo_image_surface_get_width(const_cast<cairo_surface_t*>(get()));
        }

        inline int getPhysicalWidth(void) const
        {
            return cairo_image_surface_get_stride(const_cast<cairo_surface_t*>(get())) / 4; // Note: always using 32-bit pixels
        }

        inline int getHeight(void) const
        {
            return cairo_image_surface_get_height(const_cast<cairo_surface_t*>(get()));
        }

     protected:
        cairo_surface_t * mySurface;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLinux::Surface");

    }; // class Surface;

    typedef PaCaLib::PathPtr PathPtr;
    typedef PaCaLib::DrawPtr DrawPtr;

    class Draw;

    class Target: public PaCaLib::Target
    {
        friend class PaCaLib::Target;
        friend class PaCaLinux::Draw;

        Target(int width, int height, Glesly::PixelFormat format);

     public:
        virtual ~Target();

        Surface & getSurface(void)
        {
            return mySurface;
        }

     protected:
        virtual int GetWidth(void) const override;
        virtual int GetHeight(void) const override;
        virtual const void * GetPixelData(void) const override;
        virtual int GetLogicalWidth(void) const override;
        virtual DrawPtr Draw(void) override;

        Surface mySurface;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLinux::Target");

    }; // class Target

    class Path;

    class Draw: public PaCaLib::Draw
    {
        friend class PaCaLinux::Target;
        friend class PaCaLinux::Path;

     public:
        virtual ~Draw();

        inline cairo_t * getCairo(void)
        {
            return myCairo;
        }

     protected:
        Draw(PaCaLinux::Target & target);

        virtual void Scale(float w, float h) override;
        virtual void SetLineWidth(float width) override;
        virtual void SetLineCap(PaCaLib::LineCap mode) override;
        virtual void SetColour(float r, float g, float b, float a) override;
        virtual float DrawTextInternal(float x, float y, PaCaLib::TextMode mode, const char * text, float size, float offset, float aspect = 1.0) override;
        virtual void SetTextOutlineColour(float r, float g, float b, float a = 1.0) override;
        virtual void SetTextOutline(float outline) override;
        virtual void Paint(void) override;
        virtual PathPtr NewPath(void) override;

        PaCaLinux::Target & target;

        cairo_t * myCairo;

        PangoFontDescription * myFontDescription;

        float myTextOutline;

        PaCaLib::Colour myTextOutlineColour;

        float myWidth;

        float myHeight;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLinux::Draw");

        static Threads::Mutex myTextMutex;

    }; // class PaCaLinux::Draw

    class Path: public PaCaLib::Path
    {
        friend class Draw;

     public:
        virtual ~Path();

     protected:
        Path(Draw & parent);

        virtual void Move(float x, float y) override;
        virtual void Line(float x, float y) override;
        virtual void Arc(float xc, float yc, float r, float a1, float a2) override;
        virtual void Close(void) override;
        virtual void Clear(void) override;
        virtual void Stroke(void) override;
        virtual void Fill(void) override;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLinux::Path");

        Draw & parent;

    }; // class PaCaLinux::Path

} // namespace PaCaLinux

#endif /* __SRC_PACALIB_LINUX_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
