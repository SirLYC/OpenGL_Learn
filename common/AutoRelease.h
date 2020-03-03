//
// Created by Liu Yuchuan on 2020/3/1.
//

#ifndef OPENGL_LEARN_AUTORELEASE_H
#define OPENGL_LEARN_AUTORELEASE_H


template<typename Run, typename Release>
class AutoRelease {
public:
    AutoRelease(Run &run, Release &release);

    void autoReleaseRun();

    ~AutoRelease();

private:
    Run run;
    Release release;
};

template<typename Run, typename Release>
void AutoRelease<Run, Release>::autoReleaseRun() {
    run();
}

template<typename Run, typename Release>
AutoRelease<Run, Release>::~AutoRelease() {
    release();
}

template<typename Run, typename Release>
AutoRelease<Run, Release>::AutoRelease(Run &run, Release &release):run(run), release(release) {}

template<typename Run, typename Release>
inline void autoRelease(Run run, Release release) {
    AutoRelease<Run, Release> a(run, release);
    a.autoReleaseRun();
}


#endif //OPENGL_LEARN_AUTORELEASE_H
