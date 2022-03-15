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
#include <ROOT/REveBoxSet.hxx>

using namespace ROOT::Experimental;

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
   }

};

REveBoxSet *testGUI(Int_t num = 100)
{
   auto eveMng = REveManager::Create();

   TRandom r(0);

   auto pal = new REveRGBAPalette(0, 130);

   auto q = new REveBoxSet("BoxSet");
   q->SetPalette(pal);
   q->Reset(REveBoxSet::kBT_FreeBox, kFALSE, 64);

#define RND_BOX(x) (Float_t) r.Uniform(-(x), (x))

   Float_t verts[24];
   for (Int_t i = 0; i < num; ++i)
   {
      Float_t x = RND_BOX(10);
      Float_t y = RND_BOX(10);
      Float_t z = RND_BOX(10);
      Float_t a = r.Uniform(0.2, 0.5);
      Float_t d = 0.05;
      Float_t verts[24] = {
          x - a + RND_BOX(d), y - a + RND_BOX(d), z - a + RND_BOX(d),
          x - a + RND_BOX(d), y + a + RND_BOX(d), z - a + RND_BOX(d),
          x + a + RND_BOX(d), y + a + RND_BOX(d), z - a + RND_BOX(d),
          x + a + RND_BOX(d), y - a + RND_BOX(d), z - a + RND_BOX(d),
          x - a + RND_BOX(d), y - a + RND_BOX(d), z + a + RND_BOX(d),
          x - a + RND_BOX(d), y + a + RND_BOX(d), z + a + RND_BOX(d),
          x + a + RND_BOX(d), y + a + RND_BOX(d), z + a + RND_BOX(d),
          x + a + RND_BOX(d), y - a + RND_BOX(d), z + a + RND_BOX(d)};
      q->AddBox(verts);
      q->DigitValue(r.Uniform(0, 130));
   }
   q->RefitPlex();

#undef RND_BOX

   // Uncomment these two lines to get internal highlight / selection.
   q->SetPickable(1);
   q->SetAlwaysSecSelect(1);

   eveMng->GetEventScene()->AddElement(q);

   eveMng->Show();

   eveMng->AddLocation("mydir/", "ui5");
   eveMng->SetDefaultHtmlPage("file:mydir/eventDisplay.html");
   
   auto eventMng = new EventManager();
   eventMng->SetName("EventManager");
   eveMng->GetWorld()->AddElement(eventMng);
   eveMng->GetWorld()->AddCommand("NextEvent", "sap-icon://step", eventMng, "NextEvent()");


   return q;
}
