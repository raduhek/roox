# Roox
### A small library to search multiple well defined formulas

## What ?

`Roox` was designed to perform pattern matching using user-defined rules.

The rules are defined as a logical proposition, where each operator defines the whether a pattern matches or not a text.


## How ?

There are supported only three operators now: `&`, `|` and `~`.

`&` and `|` must be binary (i.e. must have a left and right term) and must be enclosed in parentheses where there is the risk of confusion.

`roox & awesome` is valid, whereas `roox & awesome | amazing` is not. The latter should be `roox & (awesome | amazing)`


## Usage

`Roox` provides a simple HTTP with three endpoints:
 * `/add` to add a pattern for later matching
 * `/compile` some internal magic happens when you call this. You should call it after adding a batch of patterns
 * `/match` to match a text

Data for `/add` and `/match` looks as follows:

```javascript
{
  "id": <id> # a unique identifier
  "phrase": <text> # some text
}
```

Also, the `Content-Type: application/javascript` must be sent with each call (except for `/compile`).

For `/add`, `phrase` is the logical proposition, and for `/match` is the text where to look for patterns
