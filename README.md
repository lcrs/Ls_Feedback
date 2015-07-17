# Ls_Feedback
Sparks which feed the previous output frame back into the tree for recursive effects. Similar to Flame's Recursive Ops node, but you can build your own function out of whatever nodes you wish.

Be careful to only use one pair at a time, and maybe don't change resolution with abandon or they may crash :)

The matchbox shader included can be used to do cheapo fluid simulation in conjunction with the Advect shader, like this:

![demo](https://github.com/lcrs/Ls_Feedback/raw/master/Ls_Feedback.gif)
