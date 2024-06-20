#include "Tune.h"

#include "Note.h"
#include "test/lib/test.h"

using namespace sid;

int main() {
  test("==", []() {
    Tune t1, t2;
    assert("empty equals empty", t1 == t2);

    Tune simple = Tune(1, 1, 1, {});
    assert("empty does not equal non-empty", t1 != simple);

    Tune simple2 = Tune(2, 1, 1, {});
    assert("different tunes do not equal each other", simple != simple2);

    vector<unique_ptr<Track>> tracks;
    tracks.push_back(make_unique<Track>());
    tracks.back()->push_back(Note::makeRest());
    tracks.back()->push_back(Note::fromSymbol("C-----"));

    Tune order1 = Tune(1, 1, 1, std::move(tracks));
    assert("equates a tune with itself", order1 == order1);

    vector<unique_ptr<Track>> tracks2;
    tracks2.push_back(make_unique<Track>());
    tracks2.back()->push_back(Note::fromSymbol("C-----"));
    tracks2.back()->push_back(Note::makeRest());

    Tune order2 = Tune(1, 1, 1, std::move(tracks2));
    assert("does not equate tracks with different order", order1 != order2);

    vector<unique_ptr<Track>> tracksdup;
    tracksdup.push_back(make_unique<Track>());
    tracksdup.back()->push_back(Note::makeRest());
    tracksdup.back()->push_back(Note::fromSymbol("C-----"));

    vector<unique_ptr<Track>> tracksdupdup;
    tracksdupdup.push_back(make_unique<Track>());
    tracksdupdup.back()->push_back(Note::makeRest());
    tracksdupdup.back()->push_back(Note::fromSymbol("C-----"));

    Tune deepEq1 = Tune(1, 1, 1, std::move(tracksdup));
    Tune deepEq2 = Tune(1, 1, 1, std::move(tracksdupdup));
    assert("performs deep equality checks", deepEq1 == deepEq2);
  });

  return report();
}