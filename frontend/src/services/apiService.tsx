import axios from "axios";

import { API_BASE_URL } from "../utils/constants";

export const request_move = async (velA: number, velB: number) => {
  const res = await axios
    .patch(API_BASE_URL + `/move?vela=${velA}&velb=${velB}`, {
      headers: {
        "Content-Type": "application/json",
      },
    })
    .then(async (res) => {
      return { error: false, data: res };
    })
    .catch((error) => {
      return { error: true, data: error };
    });
  return res;
};
