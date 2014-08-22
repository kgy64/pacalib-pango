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

    class Target: public PaCaLib::Target
    {
     public:
        Target(int width, int height);
        virtual ~Target();

        virtual int GetWidth(void) const override;
        virtual int GetHeight(void) const override;
        virtual const void * GetPixelData(void) const override;
        virtual int GetLogicalWidth(void) const override;
        virtual void Scale(double w, double h) override;
        virtual void Stroke(void) override;
        virtual void Fill(void) override;
        virtual void FillPreserve(void) override;
        virtual void SetLineWidth(double width) override;
        virtual void Move(double x, double y) override;
        virtual void Line(double x, double y) override;
        virtual void SetLineCap(PaCaLib::LineCap mode) override;
        virtual void SetColour(double r, double g, double b) override;
        virtual void SetColour(double r, double g, double b, double a) override;
        virtual void SetColour(const PaCaLib::Colour & col) override;
        virtual void Rectangle(double x, double y, double w, double h) override;
        virtual void Arc(double xc, double yc, double r, double a1, double a2) override;
        virtual void NewPath(void) override;
        virtual void NewSubPath(void) override;
        virtual void ClosePath(void) override;
        virtual double DrawText(double x, double y, PaCaLib::TextMode mode, const char * text, double size, double aspect = 1.0) override;
        virtual void SetTextOutlineColour(double r, double g, double b, double a = 1.0) override;
        virtual void SetTextOutline(double outline) override;
        virtual void Paint(void) override;
        virtual void Paint(double alpha) override;
        virtual void Operator(PaCaLib::Oper op) override;

     protected:
        double myWidth;

        double myHeight;

        Surface mySurface;

        cairo_t * myCairo;

        PangoFontDescription * myFontDescription;

        double myTextOutline;

        PaCaLib::Colour myTextOutlineColour;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLinux::Target");

        static Threads::Mutex myTextMutex;

    }; // class Target

} // namespace PaCaLinux

#endif /* __SRC_PACALIB_LINUX_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
