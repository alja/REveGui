/// \file
/// \ingroup tutorial_eve
/// Demonstrates usage of REveBoxSet class.
///
/// \image html eve_boxset.png
/// \macro_code
///
/// \author Matevz Tadel

#include "TRandom.h"
#include <ROOT/REveElement.hxx>
#include <ROOT/REveScene.hxx>
#include <ROOT/REveManager.hxx>
#include <ROOT/REvePointSet.hxx>
#include "nlohmann/json.hpp"


using namespace ROOT::Experimental;

REvePointSet* makePointSet()
{
    TRandom &r = *gRandom;
    int npoints = 100; float s  =2;
    auto ps = new REvePointSet("fu", "", npoints);

    for (Int_t i=0; i<npoints; ++i)
       ps->SetNextPoint(r.Uniform(-s,s), r.Uniform(-s,s), r.Uniform(-s,s));

    ps->SetMarkerColor(r.Integer(700));
    ps->SetMarkerSize(3+r.Uniform(1, 7));
    ps->SetMarkerStyle(4);
    return ps;
}

class GuiInfo : public REveElement
{
public:
   int fCount{0};
   int fTotal{10};
   int fLumi{1111};

   using REveElement::WriteCoreJson;
   int WriteCoreJson(nlohmann::json &j, int rnr_offset) override
   {
      j["path"] = "data.root";
      j["count"] = fCount;
      j["total"] = fTotal;
      j["UT_PostStream"] = "UT_refresh_event_info";
      return REveElement::WriteCoreJson(j, 0);
   }
};

class EventManager : public REveElement
{
private:
   GuiInfo *fGui{nullptr};

public:
   EventManager()
   {
      SetName("EventManager");
   }

   virtual ~EventManager() {}

   void InitGuiInfo()
   {
      fGui = new GuiInfo();
      fGui->SetName("WebGuiInfo");
      gEve->GetWorld()->AddElement(fGui);
   }

   void NextEvent()
   {
      fGui->fCount++;
      fGui->StampObjProps();
      printf("At event %d\n", fGui->fCount);

      auto scene = gEve->GetEventScene();
      scene->DestroyElements();
      scene->AddElement(makePointSet());
   }
};

void
testGUI()
{
   auto eveMng = REveManager::Create();

   eveMng->GetGlobalScene()->AddElement(makePointSet());
   eveMng->GetEventScene()->AddElement(makePointSet());

   eveMng->Show();
   eveMng->GetWebWindow()->SetClientVersion("22.22");
   eveMng->AddLocation("mydir/", "ui5");
   eveMng->SetDefaultHtmlPage("file:mydir/eventDisplay.html");
   
   auto eventMng = new EventManager();
   eveMng->GetWorld()->AddElement(eventMng);
   eventMng->InitGuiInfo();
   eveMng->GetWorld()->AddCommand("NextEvent", "sap-icon://step", eventMng, "NextEvent()");
}
