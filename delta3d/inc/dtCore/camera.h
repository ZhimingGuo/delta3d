/*
 * Delta3D Open Source Game and Simulation Engine
 * Copyright (C) 2004-2005 MOVES Institute
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#ifndef DELTA_CAMERA
#define DELTA_CAMERA

// camera.h: interface for the Camera class.
//
//////////////////////////////////////////////////////////////////////


#include <dtUtil/generic.h>
#include <dtCore/refptr.h>
#include <dtCore/transformable.h>
#include <osg/Vec4>

#include <osg/Camera>

namespace dtUtil
{
   template <typename R, class TList, unsigned int size>
   class Functor;
}

namespace dtCore
{
   class DeltaWin;
   class Scene;
   class ScreenShotCallback;

   /**
    * A dtCore::Camera is a view into the Scene.  It requires a dtCore::DeltaWin to
    * render the the Scene into.  If no DeltaWin is supplied, a default DeltaWin
    * will be created and will be overridden when a valid DeltaWin is supplied
    * using SetWindow(). 
    *
    * Any part of the Scene that doesn't contain renderable geometry will be
    * drawn a solid color using the values supplied to SetClearColor().
    *
    * The Frame() method must be called once per frame.  This will update the
    * scene, then cull and draw the visual objects.
    */
   class DT_CORE_EXPORT Camera : public Transformable
   {
      DECLARE_MANAGEMENT_LAYER(Camera)

   public:

      Camera(const std::string& name = "camera");
      Camera(osg::Camera& osgCamera, const std::string& name = "camera");

   protected:

      virtual ~Camera();

   public:

      enum AutoNearFarCullingMode
      {
         NO_AUTO_NEAR_FAR = 0,      ///<no auto calculation
         BOUNDING_VOLUME_NEAR_FAR,  ///<use the scene graph bounding spheres
         PRIMITIVE_NEAR_FAR         ///<use the scene graph primitives
      };

      ///Change how the near/far culling planes get automatically calculated
      void SetNearFarCullingMode(AutoNearFarCullingMode mode);

      //Enabled or disable this Camera. Disabled Cameras will not render.
      void SetEnabled(bool enabled);

      ///Is this Camera enabled?
      bool GetEnabled() const;


      ///Get the supplied DeltaWin (could be NULL)
      DeltaWin* GetWindow()             { return (mWindow.get()); }
      const DeltaWin* GetWindow() const { return (mWindow.get()); }

      ///Use the supplied DeltaWin to draw into
      void SetWindow(DeltaWin *win);

      /**
       * Take a screen shot at end of next frame
       * @param namePrefix the prefix of the screenshot file to write
       * @return the final file name of the screenshot that will be written next frame.
       */
      const std::string TakeScreenShot(const std::string& namePrefix);

      /**
       * Set the perspective parameters of this Camera.
       * @param vfov : the vertical field of view in degrees
       * @param aspectRatio : the aspect ratio of the camera (width/height)
       * @param nearClip : the distance from the Camera to the near clipping plane
       * @param farClip : the distance from the Camera to the far clipping plane
       */
      void SetPerspectiveParams(double vfov, double aspectRatio, double nearClip, double farClip);

      /**
      * Get the perspective parameters of this Camera.
      * @param vfov : the vertical field of view in degrees
      * @param aspectRatio : the aspect ratio of the camera (width/height)
      * @param nearClip : the distance from the Camera to the near clipping plane
      * @param farClip : the distance from the Camera to the far clipping plane
      */
      void GetPerspectiveParams(double& vfov, double& aspectRatio, double& nearClip, double& farClip);

      ///Set view frustrum of camera lens
      void SetFrustum(double left, double right,
                      double bottom, double top,
                      double nearClip, double farClip);

      ///Get view frustrum in the projection matrix
      void GetFrustum(double& left, double& right,
                      double& bottom, double& top,
                      double& nearClip, double& farClip);

      ///set orthographic mode
      void SetOrtho(double left, double right,
                    double bottom, double top,
                    double nearClip, double farClip);

      ///@return HOV
      float GetHorizontalFov();

      ///@return FOV
      float GetVerticalFov();

      void SetProjectionResizePolicy( osg::Camera::ProjectionResizePolicy prp )
      {
         mOsgCamera->setProjectionResizePolicy(prp); 
      }

      osg::Camera::ProjectionResizePolicy GetProjection()
      {
         return (mOsgCamera->getProjectionResizePolicy()); 
      }

      ///takes a number from 0-1 to set as the aspect ratio
      void SetAspectRatio(double aspectRatio);

      ///@return aspect ratio
      double GetAspectRatio();

      void SetClearColor(float r, float g, float b, float a);
      void SetClearColor(const osg::Vec4& color);
      void GetClearColor(float& r, float& g, float& b, float& a);
      void GetClearColor(osg::Vec4& color);

      ///Get a non-const handle to the underlying Producer::Camera
      osg::Camera* GetOSGCamera() { return mOsgCamera.get(); }

      ///Get a const handle to the underlying Producer::Camera
      const osg::Camera* GetOSGCamera() const { return mOsgCamera.get(); }

      /**
       * Changes the multiplier applied to the Level of Detail distance calculation when rendering.
       * For example, setting this to 2 would double the distance used when picking the LOD.  This means
       * the detail will decrease twice as fast.
       */
      void SetLODScale(float newScale);

      /// @return the multiplier applied to the Level of Detail distance calculation when rendering.
      float GetLODScale() const;

      /**
      * Supply the Scene this Camera has been added to. Normally this
      * is done inside dtCore::Scene. So you should probably never have
      * to call this.
      *
      * @param scene The Scene to which this Camera is being added to.
      */
      virtual void AddedToScene(Scene* scene);

      /**
       * Gets the position on the screen into the outScreenPos and returns whether or not the position is on the screen
       * in normalized coordinates.  outScreenPos.x is the horizontal position 0 - 1 from left to right.  outScreenPos.y
       * is the veritcal position O - 1 from bottom to top.  outScreenPos.z is the depth from 0 - 1 from the near clipping
       * plane to the far clipping plane.  The values for anything values may be greater or less of the position
       * given is off the screen or behind the camera..
       */
      bool ConvertWorldCoordinateToScreenCoordinate(const osg::Vec3d& worldPos, osg::Vec3d& outScreenPos) const;     

      typedef dtUtil::Functor<void, TYPELIST_1(dtCore::Camera&), 4 * sizeof(void*)> FrameSyncCallback;

      /**
       * Adds a callback that fires whenever the this camera is updated on FrameSync.  This makes it easy to
       * add code that will add camera specific uniforms, or do operations in screen space since the camera will be in its
       * final state prior to rendering.
       *
       * An example call from inside class "A" with a method with signature "void OnCameraSync(Camera& camera)" would be:
       *
       * Camera::AddFrameSyncCallback(this, FrameSyncCallback(this, &A::OnCameraSync))
       *
       * @param keyObject this object will be held onto with an observer pointer.
       *                  When this object is deleted, the callback will be removed.
       *                  It's also handy when removing callbacks directly.
       * @param callback a functor to call. See the typedef above FrameSyncCallback.
       */
      static void AddFrameSyncCallback(osg::Referenced& keyObject, FrameSyncCallback callback);
      static void RemoveFrameSyncCallback(osg::Referenced& keyObject);

      // Updates the view matrix via the transformed position.  This is called from FrameSynch.
      void UpdateViewMatrixFromTransform();

   protected:

      ///Override for FrameSynch
      virtual void FrameSynch(const double deltaFrameTime);

      /// Base override to receive messages.
      /// This method should be called from derived classes
      /// @param data the message to receive
      virtual void OnMessage(MessageData* data);

      /// Call all of the static frame sync callbacks using this camera.
      void CallFrameSyncCallbacks();

   private:

      // Disallowed to prevent compile errors on VS2003. It apparently
      // creates this functions even if they are not used, and if
      // this class is forward declared, these implicit functions will
      // cause compiler errors for missing calls to "ref".
      Camera& operator=(const Camera&);
      Camera(const Camera&);

      void OnWindowChanged();

      ///Common constructor functionality
      void Ctor();

      RefPtr<osg::Camera> mOsgCamera; // Handle to the osg Camera
      RefPtr<DeltaWin> mWindow; // The currently assigned DeltaWin

      bool mAddedToSceneGraph;
      bool mEnable;
      RefPtr<ScreenShotCallback> mScreenShotTaker;

      osg::Node::NodeMask mEnabledNodeMask; ///<The last known node mask corresponding to "enabled"
   };
}

#endif // DELTA_CAMERA
