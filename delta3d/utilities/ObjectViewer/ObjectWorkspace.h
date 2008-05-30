/*
* Delta3D Open Source Game and Simulation Engine 
* Copyright (C) 2008 MOVES Institute 
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

#ifndef DELTA_OBJECTWORKSPACE
#define DELTA_OBJECTWORKSPACE

#include <QtGui/QMainWindow>

///////////////////////////////////////////////////////////////////////////////

class ResourceDock;
class QAction;
class QToolBar;

namespace dtQt
{
   class OSGAdapterWidget; 
}

///////////////////////////////////////////////////////////////////////////////

class ObjectWorkspace : public QMainWindow
{
   friend class Delta3DThread;
   Q_OBJECT
public:
   ObjectWorkspace();
   ~ObjectWorkspace();

   dtQt::OSGAdapterWidget* GetGLWidget() { return mGLWidget;     }
   QObject* GetResourceObject();          

   virtual void dragEnterEvent(QDragEnterEvent *event);
   virtual void dropEvent(QDropEvent *event);
   
signals:
   void FileToLoad(const QString&);  
   void LoadShaderDefinition(const QString &);
   void LoadGeometry(const QString &);
   void ReloadShaderDefinition(const QString &);
   void StartAction(unsigned int, float, float);
   
   void ToggleGrid(bool shouldShow);

public slots:
   
   void OnInitialization(); 
   void OnToggleShadingToolbar(); 
   void OnRecompileClicked();
	
private:
   void CreateMenus();
   void CreateActions();
   void CreateToolbars();
   void UpdateResourceLists();
   void LoadObjectFile(const QString &filename);

   // File menu
   QAction *mLoadShaderDefAction;
   QAction *mLoadGeometryAction;
   QAction *mChangeContextAction;
   QAction *mExitAct;
  
   // Toolbar
   QAction *mWireframeAction; 
   QAction *mShadedAction;    
   QAction *mShadedWireAction;
   QAction *mGridAction;
   QAction *mDiffuseLightAction;
   QAction *mPointLightAction;
   QAction *mRecompileAction;

   QToolBar *mDisplayToolbar;    
   QToolBar *mShaderToolbar;

   ResourceDock *mResourceDock;

   std::string mContextPath;
   QString mShaderDefinitionName;

   dtQt::OSGAdapterWidget* mGLWidget;

private slots:  

   // File menu callbacks
   void OnLoadShaderDefinition();
   void OnLoadGeometry();
   void OnLoadGeometry(const std::string &fullName);
   void OnChangeContext();
   
   void OnToggleGridClicked(bool toggledOn);

   std::string GetContextPathFromUser();
   void SaveCurrentContextPath();
};

#endif // DELTA_OBJECTWORKSPACE
