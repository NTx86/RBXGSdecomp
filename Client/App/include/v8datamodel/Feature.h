#pragma once
#include "v8datamodel/JointInstance.h"

// TODO: implement classes

namespace RBX
{
    extern const char* sFeature;
    extern const char* sHole;
    extern const char* sVelocityMotor;

    class Feature : public DescribedNonCreatable<Feature, Instance, &sFeature>,
                    public IRenderable
    {
    public:
        enum TopBottom {
            TOP = 0,
            CENTER_TB = 1,
            BOTTOM = 2,
        };

        enum LeftRight {
            LEFT = 0,
            CENTER_LR = 1,
            RIGHT = 2
        };

        enum InOut {
            EDGE = 0,
            INSET = 1,
            CENTER_IO = 2
        };

        enum InOutZ {
            Z_IN = 0,
            Z_OUT = 1
        };

        NormalId faceId;
        TopBottom topBottom;
        LeftRight leftRight;
        InOut inOut;

    private:
        virtual bool askSetParent(const Instance*) const;
        virtual bool shouldRender3dAdorn() const;
        virtual void render3dSelect(Adorn*, SelectState);

    protected:
        bool getRenderCoord(G3D::CoordinateFrame&) const;
        virtual Feature::InOutZ getCoordOrientation() const;

    public:
        Feature(const Feature&);
        Feature();
        virtual ~Feature();

        NormalId getFaceId() const;
        void setFaceId(NormalId);

        Feature::TopBottom getTopBottom() const;
        void setTopBottom(TopBottom);

        Feature::LeftRight getLeftRight() const;
        void setLeftRight(LeftRight);

        Feature::InOut getInOut() const;
        void setInOut(InOut);

        G3D::CoordinateFrame computeLocalCoordinateFrame() const;
    };

    class Hole : public DescribedCreatable<Hole, Feature, &sHole>
    {
    private:
        virtual void render3dAdorn(Adorn*);
        virtual Feature::InOutZ getCoordOrientation() const;

    public:
        Hole(const Hole&);
        Hole();
        virtual ~Hole();
    };

    class VelocityMotor : public DescribedCreatable<VelocityMotor, JointInstance, &sVelocityMotor>
    {
    private:
        boost::shared_ptr<Hole> hole;
        boost::signals::scoped_connection holeAncestorChanged;

        const MotorJoint* motorJoint() const;
        MotorJoint* motorJoint();

        void onEvent_HoleAncestorChanged();
        void setPart(int, Feature*);

        virtual bool askSetParent(const Instance*) const;
        virtual void onAncestorChanged(const AncestorChanged&);

    public:
        VelocityMotor(const VelocityMotor&);
        VelocityMotor();
        virtual ~VelocityMotor();

        Hole* getHole() const;
        void setHole(Hole*);

        float getMaxVelocity() const;
        void setMaxVelocity(float);

        float getDesiredAngle() const;
        void setDesiredAngle(float);

        float getCurrentAngle() const;
        void setCurrentAngle(float);
    };
}
