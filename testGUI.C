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

class EventManager : public REveElement
{
private:
   int  fCount{0};

public:
   EventManager()
   {
   }

   virtual ~EventManager() {}

   void NextEvent()
   {
      fCount++;
      printf("At event %d\n", fCount);

    auto scene = gEve->GetEventScene();
    scene->DestroyElements();
    scene->AddElement(makePointSet());

   }

};

void testGUI()
{
   auto eveMng = REveManager::Create();

   TRandom r(0);


   eveMng->GetGlobalScene()->AddElement(makePointSet());
   eveMng->GetEventScene()->AddElement(makePointSet());

   eveMng->Show();

   eveMng->AddLocation("mydir/", "ui5");
   eveMng->SetDefaultHtmlPage("file:mydir/eventDisplay.html");
   
   auto eventMng = new EventManager();
   eventMng->SetName("EventManager");
   eveMng->GetWorld()->AddElement(eventMng);
   eveMng->GetWorld()->AddCommand("NextEvent", "sap-icon://step", eventMng, "NextEvent()");
}
