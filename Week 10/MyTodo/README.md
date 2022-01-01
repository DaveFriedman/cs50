# Dave's TODO app!
#### Video Demo:  https://youtu.be/9yYJyNzhZ_I
#### Description:
Hi cs50!
I've made a quick and dirty TODO app to help manage my time.
I had quite some trouble at first with this project but did eventually manage to
pull through.

I started down a few false paths, at first attempting to create a CRUD (Create,
Read, Update, Delete) app all by myself, learning best practices on the fly.
This proved to be a poor choice. I found myself stuck many times, first with 
aspects of SQLAlchemy, then attempting to just work directly with sqlite3.
Low on time, I chose to use cs50's sql functions, and found success with that.

I also attempted to do my own form validation. After attempting to explore best
practices, I found WTForms. I then found myself searching for flask boilerplate
code templates. However, after a few unsuccessful tries, and other claims on my 
time, I chose to skip the overhead and get the project done.

Using my cs50 Finance work as a foundation, I adapted that code to TODO, and 
found success.  While I don't regret exploring so many rabbit holes early on,
I'm rather pleased that my TODO app builds incrementally on my earlier work.

I have a very simple MVC (Model View Controller) design. The model is the 
database, which is just users and tasks. However, the database design is simple
enough to be quite expandable for a motivated user. I am that user, and I plan 
to continue work on this app. The Views are the different pages available to 
users - your tasks and your account settings, at this point. The controller is
the app itself, using it's logic to produce a coherent product.

One struggle I had was getting the Mark Done button to work. Each button takes a
jinja variable as it's value, and that value is the title of the task. In order
to mark a task as complete, you must send that tasks's title (aka, the task 
name) to the controller in order to update the database with the "isdone" flag.
Initially I wanted the button to say "Done!". Unfortunately, the button's 
text display comes from it's value= attribute and I struggled to find a way
around that. My solution was to design the task list so that the task title
and the mark complete buttons are the same design element. Ultimately, this 
worked out nicely because it saves horizontal space and allows for more 
efficient presentation of information. Neccessity is the mother of invention.

At this point I'm quite struggling to hit the length requirement for this 
readme, I think I've said everything I want to say. Unfortunately, there might
be a little more length needed.

One thing I'd like to do with this app is eventually expand it to use 3rd-party
authentication providers, like Log In With Google or Log In With Facebook. I 
think that's a very convenient way of handling authentication for both the user
and the developer. Fewer passwords to remember, fewer security gotchas to worry
about, and so forth. Perhaps then I'll build the app to allow it to sync with
other task apps, like Google Tasks or ToDoist, etc. Microsoft also has a good 
task app.


Thanks!
Dave