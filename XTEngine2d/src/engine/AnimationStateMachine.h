#pragma once


class AnimationStateMachine
{
public:
    void SetAnimator(Animator* a)
    {
        animator = a;
    }

    void SetFloat(const std::string& name, float value)
    {
        floats[name] = value;
    }

    void Update()
    {
        float speed = floats["speed"];

        if (speed > 0.1f)
            animator->Play("Run");
        else
            animator->Play("Idle");
    }

private:
    std::unordered_map<std::string, float> floats;
    Animator* animator = nullptr;
};