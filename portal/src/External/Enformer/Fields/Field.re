/** Field managers  */

/** A generic wrapper for the individual fields
 *
 * A group is a subset of fields that work together.
 *
 * An example is if we want to break a large set of forms into sections. Each section can be validated and
 * saved individually.
  */
type fieldType =
  | Group(list(t))
  | Button(Button.t)
  | Text(Text.t)
and t = {
  guid: string,
  fieldType,
  common: Common.t,
};
