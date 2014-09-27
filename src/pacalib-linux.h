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
#include <pacalib/pacalib.h>
#include <Debug/Debug.h>

#include <pango/pangocairo.h>
#include <boost/shared_ptr.hpp>

namespace PaCaLinux
{
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
        Surface(int width, int height);
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

        static const char * GetErrorMessage(cairo_status_t status);

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLinux::Surface");

    }; // class Surface;

    typedef PaCaLib::PathPtr PathPtr;

    class Path;

    class Target: public PaCaLib::Target
    {
        friend class Path;

     public:
        Target(int width, int height);
        virtual ~Target();

        virtual int GetWidth(void) const override;
        virtual int GetHeight(void) const override;
        virtual const void * GetPixelData(void) const override;
        virtual int GetLogicalWidth(void) const override;
        virtual void Scale(float w, float h) override;
        virtual void SetLineWidth(float width) override;
        virtual void Move(float x, float y) override;
        virtual void Line(float x, float y) override;
        virtual void SetLineCap(PaCaLib::LineCap mode) override;
        virtual void SetColour(float r, float g, float b) override;
        virtual void SetColour(float r, float g, float b, float a) override;
        virtual void SetColour(const PaCaLib::Colour & col) override;
        virtual void Rectangle(float x, float y, float w, float h) override;
        virtual void Arc(float xc, float yc, float r, float a1, float a2) override;
        virtual float DrawTextInternal(float x, float y, PaCaLib::TextMode mode, const char * text, float size, float offset, float aspect = 1.0) override;
        virtual void SetTextOutlineColour(float r, float g, float b, float a = 1.0) override;
        virtual void SetTextOutline(float outline) override;
        virtual void Paint(void) override;
        virtual void Paint(float alpha) override;
        virtual void Operator(PaCaLib::Oper op) override;
        virtual PathPtr NewPath(void) override;

     protected:
        float myWidth;

        float myHeight;

        Surface mySurface;

        cairo_t * myCairo;

        PangoFontDescription * myFontDescription;

        float myTextOutline;

        PaCaLib::Colour myTextOutlineColour;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLinux::Target");

        static Threads::Mutex myTextMutex;

    }; // class Target

    class Path: public PaCaLib::Path
    {
        friend class Target;

     public:
        virtual ~Path();

     protected:
        Path(Target & parent);

        virtual void Move(float x, float y) override;
        virtual void Line(float x, float y) override;
        virtual void Arc(float xc, float yc, float r, float a1, float a2) override;
        virtual void Close(void) override;
        virtual void Clear(void) override;
        virtual void SetLineWidth(float width) override;
        virtual void SetLineCap(PaCaLib::LineCap mode) override;
        virtual void Fill(void) override;
        virtual void Stroke(void) override;
        virtual void SetColour(float r, float g, float b) override;
        virtual void SetColour(float r, float g, float b, float a) override;
        virtual void SetColour(const PaCaLib::Colour & col) override;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaAndroid::Path");

        Target & parent;

    }; // class PaCaLinux::Path

} // namespace PaCaLinux

#endif /* __SRC_PACALIB_LINUX_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
