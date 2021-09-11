## Pull Request Etiquette

### Learn about the Pull Request Technique

Learn about what a PR is, and how it works with GitHub. There's some great guides here [https://docs.github.com/en/github/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/about-pull-requests](https://docs.github.com/en/github/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/about-pull-requests)

And then some general guidance on how to write the perfect PR. [https://github.blog/2015-01-21-how-to-write-the-perfect-pull-request](https://github.blog/2015-01-21-how-to-write-the-perfect-pull-request)

Now assuming that you have first forked a repo, here is what you should do in that fork that you own:

* Create a branch. Name it something related, but short with your name. Like name/new-feature
	* Isolate your modifications in a branch. Don't create a pull request from main, where you could be tempted to accumulate and mix several modifications at once.
* Rebase that branch. We won't accept PRs that are not rebased and squashed. 
	* Even if you already did a pull request from that branch, rebasing it on top of origin/main (making sure your patch is still working) will update the pull request automagically (no need to click on anything)
* Update that branch.
	* If your pull request is rejected, you simply can add new commits, and/or redo your history completely: it will activate your existing pull request again.
	* **_Focus_** that branch: i.e., make its topic "tight", don't modify thousands of files, only add or make small changes.
* Delete that branch. 
	* Once accepted, you can safely delete that branch on your fork (and git remote prune origin). The GitHub GUI will propose for you to delete your branch in your pull-request page.

### Sensible commit history.

When adding history to a file, make it sensible because this will be preserved for all time. Make it count.

Some good examples:  

    [docs/enhancement] - heyigor/miniBAE - updates in documentation (overall grammar, layout, et. al)

    Adding Flux Capaciter Feature. To enable play a Bb really loud.


