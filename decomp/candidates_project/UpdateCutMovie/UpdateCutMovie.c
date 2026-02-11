#define rate_val 1.0f

void UpdateCutMovie(void) {
    if (((cutmovie_hold == 0) || (CutAudio[cutworldix] == -1)) || (NuSoundKeyStatus(4) != 1)) {
        CutInst[cutworldix]->rate = 0.0f;
    }
    else {
        cutmovie_hold = 0;
        SetCutMovieRate();
    }
    NuGCutSceneSysUpdate(Paused);
    return;
}
