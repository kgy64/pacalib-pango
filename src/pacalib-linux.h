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
        Surface(const Glesly::Target2D & source);
        VIRTUAL_IF_DEBUG ~Surface();

        void CopyFrom(const Glesly::Target2D & other);

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
            cairo_surface_t * surface = const_cast<cairo_surface_t*>(get());
            cairo_format_t format = cairo_image_surface_get_format(surface);
            return cairo_image_surface_get_stride(surface) / (format == CAIRO_FORMAT_RGB16_565 ? 2 : 4); // Note: using only 2 or 4 byte sizes
        }

        inline int getHeight(void) const
        {
            return cairo_image_surface_get_height(const_cast<cairo_surface_t*>(get()));
        }

        inline Glesly::PixelFormat GetPixelFormat(void) const
        {
            return myPixelFormat;
        }

        inline static cairo_format_t GetCairoPixelFormat(Glesly::PixelFormat format)
        {
            switch (format) {
                case Glesly::FORMAT_RGB_565:
                    return CAIRO_FORMAT_RGB16_565;
                break;
                case Glesly::FORMAT_RGBA_8888:
                    return CAIRO_FORMAT_ARGB32;
                break;
                default:
                    DEBUG_OUT("Having unknown (unused) pixel format " << (int)format);
                break;
            }
            return CAIRO_FORMAT_INVALID;
        }

     protected:
        Glesly::PixelFormat myPixelFormat;

        cairo_surface_t * mySurface;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLinux::Surface");

        inline cairo_format_t GetCairoPixelFormat(void) const
        {
            return GetCairoPixelFormat(myPixelFormat);
        }

    }; // class PaCaLinux::Surface

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
        virtual Glesly::Target2D & operator=(const Glesly::Target2D & other) override;

        virtual int GetWidth(void) const override;
        virtual int GetHeight(void) const override;
        virtual const void * GetPixelData(void) const override;
        virtual Glesly::PixelFormat GetPixelFormat(void) const override;
        virtual int GetLogicalWidth(void) const override;
        virtual DrawPtr Draw(void) override;

        Surface mySurface;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLinux::Target");

    }; // class PaCaLinux::Target

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

        void CopyFrom(const Surface & source);

        virtual void Scale(float w, float h) override;
        virtual void SetLineWidth(float width) override;
        virtual void SetLineCap(PaCaLib::LineCap mode) override;
        virtual void SetColour(float r, float g, float b, float a) override;
        virtual void SetColourCompose(PaCaLib::ColourCompose mode) override;
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
