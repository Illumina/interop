# Contributing to Illumina InterOp

Illumina InterOp is an open source community project intended to make parsing and using metrics produced by a 
sequencing run easier. We encourage users to contribute in a number of ways:

  - Post questions, bugs or feature requests on our [Issues] tracker. Please make sure to label them appropriately.
  - Contribute to Illumina InterOp Documentation by adding Wiki examples showing how you use the code
  - Blog about Illumina InterOp, tell others about how great it is.
  
[Issues]: https://github.com/Illumina/interop/labels

If think working *with* Illumina InterOp is fun, just wait until you start working *on* it. We are passionate
about turning users into contributors, and provide several ways for you to contribute to Illumina InterOp:

  - Report bugs on our [Issues] tracker
  - Share your ideas on how to improve Illumina InterOp on our community [Issues] page
  - Submit pull requests to fix bugs or make *small* improvements
  - Improve the documentation or add unit tests

We welcome large improvements to Illumina InterOp, but we want to use your time efficiently. Please, before you begin
on any large improvement, such as adding support for new language, present your idea on the [Issues] page. We will
happily work with you to plan out the minimum necessary work required to get your changes into the source repository.
This includes unit tests, examples and documentation. If you feel that you cannot meet these requirements, then you 
can always distribute your improvement as a fork of the Illumina InterOp repository.

## Advice to new contributors

  - *Write some documentation*
    
    Illumina InterOp's documentation is just great, but it can always use some improvement. See a typo? Should something
    be clarified? Be bold, and submit a documentation pull request.
  
  - *Start small*
  
    Everything you submit needs to be thoroughly reviewed, documented and tested. For large changes this could take 
    considerable time. Try to break things up into smaller change sets with comments on the overall goal. 
  
  - *If you are going to engage in a big task, make sure your idea is supported first*
    
    This means ensuring the proposed feature is accepted by the core team or that the bug is real.
  
  - *Be patient and wait for feedback*
    
    Submit one or two pull requests at a time and wait for feedback.

## Working with Git and GitHub

After you install git, the very first thing you should do is setup your name and email

~~~~~~~~~{.sh}
$ git config --global user.name "Your Real Name"
$ git config --global user.email "you@email.com"
~~~~~~~~~

It is important for us to track who made what changes.

## Including a change log in your pull request

It is highly recommended that you include a change log with your pull request. Ideally, this will only be a single
item. Also your commit messages should be meaningful and correspond to the change log. You can create a change
log from your git commits s follows:

~~~~~~~~~{.sh}
git log v1.0.0..HEAD --oneline --decorate --no-merges
~~~~~~~~~

In the above example, v1.0.0 was the last tag, you should replace this value with the actual last tag.

## Linking to our git hooks

Windows: 

~~~~~~~~~{.bat}
mklink .git\hooks\pre-commit <abs-path>\tools\hooks\pre-commit.sh
~~~~~~~~~

Linux:

~~~~~~~~~{.sh}
ln -s tools/hooks/pre-commit.sh .git/hooks/pre-commit
~~~~~~~~~

## Debugging Unit Tests

The unit tests use the Google Test Framework. The following flags may help with debugging:

 - `--gtest_catch_exceptions=0`: Ensure GTest throws an exception
 - `--gtest_break_on_failure`: Ensure GTest stopps on the first failure
 
 
