/*
 * Delta3D Open Source Game and Simulation Engine
 * Copyright (C) 2009 MOVES Institute
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
 
#ifndef environmentmanager_h__
#define environmentmanager_h__

#include <dtInspectorQt/imanager.h>
#include <QtCore/QObject>
#include <dtCore/observerptr.h>
#include <dtCore/environment.h>

namespace Ui
{
   class InspectorWidget;
}

namespace dtInspectorQt
{
   ///Handles the properties of dtCore::Environment
   class EnvironmentManager : public IManager
   {
      Q_OBJECT

   public:
      EnvironmentManager(Ui::InspectorWidget &ui);
      virtual ~EnvironmentManager();

      virtual void OperateOn(dtCore::Base* b);

   protected slots:

   private:
      Ui::InspectorWidget* mUI;
      dtCore::ObserverPtr<dtCore::Environment> mOperateOn;

   };
}  
#endif //environmentmanager_h__
