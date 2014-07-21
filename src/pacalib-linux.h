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

#include <pacalib/pacalib.h>
#include <Debug/Debug.h>

#include <boost/shared_ptr.hpp>

namespace PaCaLinux
{
    class Surface
    {
     public:
        Surface(int width, int height);
        VIRTUAL_IF_DEBUG ~Surface();

        inline void * getData(void)
        {
            return myData.get();
        }

        inline const void * getData(void) const
        {
            return myData.get();
        }

        inline int getWidth(void) const
        {
            return myWidth;
        }

        inline int getPhysicalWidth(void) const
        {
            return myWidth;
        }

        inline int getHeight(void) const
        {
            return myHeight;
        }

        void DrawText(const char * text, float size);

     protected:
        struct argb
        {
            uint8_t a, r, g, b;

        }; // struct PaCaLinux::Surface::argb

        int myWidth;

        int myHeight;

        boost::scoped_array<argb> myData;

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLinux::Surface");

    }; // class Surface;

    class Target: public PaCaLib::Target
    {
        Surface mySurface;

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

     private:
        SYS_DEFINE_CLASS_NAME("PaCaLinux::Target");

    }; // class Target

} // namespace PaCaLib

#endif /* __SRC_PACALIB_LINUX_H_INCLUDED__ */

/* * * * * * * * * * * * * End - of - File * * * * * * * * * * * * * * */
