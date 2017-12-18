module History = {
  type t;
  module Location = {
    type t;
    [@bs.get] external pathname : t => string = "";
    [@bs.get] external search : t => string = "";
    [@bs.get] external hash : t => string = "";
    [@bs.get] [@bs.return null_undefined_to_opt] external key : t => option(string) = "";
  };
  [@bs.string]
  type action = [ | `PUSH | `REPLACE | `POP];
  [@bs.get] external length : t => int = "";
  [@bs.get] external action : t => action = "";
  [@bs.get] external location : t => string = "";
  [@bs.send] external listen : (t, ~location: Location.t, ~action: action, unit) => unit = "";
  /* TODO: state typing */
  module State = {
    type t;
  };
  [@bs.send] external push : (t, ~url: string, ~state: list(State.t)) => unit = "";
  [@bs.send] external replace : (t, ~url: string, ~state: list(State.t)) => unit = "";
  [@bs.send] external go : (t, ~jumps: int) => unit = "";
  [@bs.send] external goBack : t => unit = "";
  [@bs.send] external goForward : t => unit = "";
};

type getUserConfirmation = (~path: string, ~confirmation: bool) => unit;

[@bs.deriving jsConverter] type browserHistoryOpt = {
  basename: string,
  forceRefresh: bool,
  keyLength: int,
  getUserConfirmation
};

type jsBrowserHistoryOpt = Js.t({.
  basename: string,
  forceRefresh: bool,
  keyLength: int,
  getUserConfirmation
});

[@bs.module "history/createBrowserHistory"] external createBrowserHistory : jsBrowserHistoryOpt => History.t = "default";
let createBrowserHistory : browserHistoryOpt => History.t = (opt) => createBrowserHistory(browserHistoryOptToJs(opt));

type memoryHistoryOpt = {
  initialEntries: list(string),
  initialIndex: int,
  keyLength: int
};

type jsMemoryHistoryOpt = Js.t({.
  initialEntries: array(string),
  initialIndex: int,
  keyLength: int
});

let memoryHistoryOptToJs = ({ initialEntries, initialIndex, keyLength }) =>
  [%bs.obj { initialEntries: Array.of_list(initialEntries), initialIndex, keyLength}];

[@bs.module "history/createMemoryHistory"] external createMemoryHistory : jsMemoryHistoryOpt => History.t = "default";
let createMemoryHistory : memoryHistoryOpt => History.t = (opt) => createMemoryHistory(memoryHistoryOptToJs(opt));

[@bs.deriving jsConverter] type hashHistoryOpt = {
  basename: string,
  hashType: string,
  getUserConfirmation
};

type jsHashHistoryOpt = Js.t({.
  basename: string,
  hashType: string,
  getUserConfirmation
});

[@bs.module "history/createHashHistory"] external createHashHistory : jsHashHistoryOpt => History.t = "default";
let createHashHistory : hashHistoryOpt => History.t = (opt) => createHashHistory(hashHistoryOptToJs(opt));