/** Sub-grouping of fields
 *
 * This is for items in a form which interact with each other, such as toggling the address display based on
 * country.
 * */;

type groupType =
  | Validation
  | PartialForm;

type t = {groupType};
